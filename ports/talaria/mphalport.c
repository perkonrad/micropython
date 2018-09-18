#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <kernel/os.h>
#include <kernel/io.h>
#include <kernel/cpu.h>
#include "kernel/dma.h"
#include "kernel/uart.h"
#include "kernel/hwreg.h"
#include "kernel/gpio.h"
#include "kernel/interrupt.h"
#include "kernel/resource.h"
#include <kernel/hostio.h>

#define TX 0
#define RX 1

static uint8_t dma_buffer[1<<10]; /* Size must be a power of 2 */
static uint8_t rx_buf_dma;

struct fifo {
    struct os_semaphore rx_sem;
    uint8_t buf[256];
    uint8_t wr_ptr;
    uint8_t rd_ptr;
};

static struct fifo rx_fifo;

static struct dma_desc rx_desc;

static int
irq_rx(uint32_t irqno, void *arg)
{
    rx_fifo.buf[rx_fifo.wr_ptr] = rx_buf_dma;
    rx_fifo.wr_ptr = (rx_fifo.wr_ptr+1) % 256;

    os_sem_post( &rx_fifo.rx_sem );
    return IRQ_HANDLED;
}


void
raw_console_init(void)
{
    uart_init(921600);

    // Setup gpio mux
    os_gpio_mux_sel(GPIO_MUX_SEL_UART_TXD, 1);
    os_gpio_mux_sel(GPIO_MUX_SEL_UART_RXD, 2);

    dma_start_circular(&hw_uart->dma_ctrl[TX], dma_buffer, sizeof dma_buffer);

    rx_fifo.wr_ptr = 0;
    rx_fifo.rd_ptr = 0;

    os_sem_init( &rx_fifo.rx_sem, 0 );
    os_attach_irq(IRQ_UART_RX, irq_rx);

    dma_map_buffer(&rx_desc, &rx_buf_dma, sizeof(rx_buf_dma), DESC_RX|DESC_IRQ);
    rx_desc.next = &rx_desc;

    dma_stop(&hw_uart->dma_ctrl[RX]);
    dma_start_desc_byte(&hw_uart->dma_ctrl[RX], &rx_desc);
}

void
raw_console_flush(void)
{
    while (dma_circular_free(&hw_uart->dma_ctrl[TX]) != sizeof dma_buffer - 1)
        ;
}

static inline uint32_t
raw_console_putc(uint32_t curr, int c)
{
    dma_buffer[curr++] = c;
    return curr % sizeof dma_buffer;
}

static void
raw_console_write(const char *s, size_t len)
{
    os_irqstate_t flags = cpu_irq_disable();
    uint32_t curr = mmio_rd16(&hw_uart->dma_ctrl[TX].curr);

    while (len--)
        curr = raw_console_putc(curr, *s++);
    mmio_wr16(curr, &hw_uart->dma_ctrl[TX].curr);
    cpu_irq_restore(flags);
}


//


void mp_hal_stdout_tx_strn(const char *str, size_t len) {
    raw_console_write(str, len);
}

void mp_hal_stdout_tx_str(const char *str) {
    mp_hal_stdout_tx_strn(str, strlen(str));
}

// Efficiently convert "\n" to "\r\n"
void mp_hal_stdout_tx_strn_cooked(const char *str, size_t len)
{
    const char *last = str;
    while (len--) {
        if (*str == '\n') {
            if (str > last) {
                mp_hal_stdout_tx_strn(last, str - last);
            }
            mp_hal_stdout_tx_strn("\r\n", 2);
            ++str;
            last = str;
        } else {
            ++str;
        }
    }
    if (str > last) {
        mp_hal_stdout_tx_strn(last, str - last);
    }
}

int mp_hal_stdin_rx_chr(void)
{
    os_sem_wait( &rx_fifo.rx_sem );

    uint8_t tmp = rx_fifo.buf[rx_fifo.rd_ptr];

    rx_fifo.rd_ptr = (rx_fifo.rd_ptr+1) % 256;

    return tmp;
}
