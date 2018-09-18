#include "py/nlr.h"
#include "py/obj.h"
#include "py/mpprint.h"
#include "py/runtime.h"
#include "py/binary.h"

#include "kernel/gpio.h"

#include <stdio.h>

#define printf(...) mp_printf (&mp_plat_print, __VA_ARGS__)


///////////////////////////////////////////////////////////////////////////////
// gpio class
///////////////////////////////////////////////////////////////////////////////

typedef struct _talaria_gpio_obj_t {
    mp_obj_base_t base;
    uint8_t pin_number;
} talaria_gpio_obj_t;


extern const mp_obj_type_t talaria_gpio_type;

mp_obj_t
talaria_gpio_make_new( const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args )
{
    mp_arg_check_num(n_args, n_kw, 1, 1, true);

    talaria_gpio_obj_t *self = m_new_obj(talaria_gpio_obj_t);
    self->base.type  = &talaria_gpio_type;
    self->pin_number = mp_obj_get_int(args[0]);

    os_gpio_request(GPIO_PIN(self->pin_number));

    return MP_OBJ_FROM_PTR(self);
}

static void
talaria_gpio_print( const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind )
{
    talaria_gpio_obj_t *self = MP_OBJ_TO_PTR(self_in);

    mp_printf(print, "gpio(%u)", self->pin_number);
}

static mp_obj_t
talaria_gpio_get(mp_obj_t self_in)
{
    talaria_gpio_obj_t *self = MP_OBJ_TO_PTR(self_in);
    return MP_OBJ_NEW_SMALL_INT(os_gpio_get_value(GPIO_PIN(self->pin_number)) ? 1:0);
}

static mp_obj_t
talaria_gpio_set(mp_obj_t self_in)
{
    talaria_gpio_obj_t __unused *self = MP_OBJ_TO_PTR(self_in);

    uint32_t mask = GPIO_PIN(self->pin_number);

    os_gpio_set_output(mask);
    os_gpio_set_value(mask, 0);

    return mp_const_none;
}

static mp_obj_t
talaria_gpio_clr(mp_obj_t self_in)
{
    talaria_gpio_obj_t __unused *self = MP_OBJ_TO_PTR(self_in);

    uint32_t mask = GPIO_PIN(self->pin_number);

    os_gpio_set_output(mask);
    os_gpio_set_value(0, mask);

    return mp_const_none;
}

static mp_obj_t
talaria_gpio_zzz(mp_obj_t self_in)
{
    talaria_gpio_obj_t __unused *self = MP_OBJ_TO_PTR(self_in);

    uint32_t mask = GPIO_PIN(self->pin_number);

    os_gpio_set_input(mask);

    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_1(talaria_gpio_get_obj, talaria_gpio_get);
MP_DEFINE_CONST_FUN_OBJ_1(talaria_gpio_set_obj, talaria_gpio_set);
MP_DEFINE_CONST_FUN_OBJ_1(talaria_gpio_clr_obj, talaria_gpio_clr);
MP_DEFINE_CONST_FUN_OBJ_1(talaria_gpio_zzz_obj, talaria_gpio_zzz);

static const mp_rom_map_elem_t talaria_gpio_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_get), MP_ROM_PTR(&talaria_gpio_get_obj) },
    { MP_ROM_QSTR(MP_QSTR_set), MP_ROM_PTR(&talaria_gpio_set_obj) },
    { MP_ROM_QSTR(MP_QSTR_clr), MP_ROM_PTR(&talaria_gpio_clr_obj) },
    { MP_ROM_QSTR(MP_QSTR_z),   MP_ROM_PTR(&talaria_gpio_zzz_obj) },
};
static MP_DEFINE_CONST_DICT(talaria_gpio_locals_dict, talaria_gpio_locals_dict_table);

const mp_obj_type_t talaria_gpio_type = {
    { &mp_type_type },
    .name        = MP_QSTR_gpio,
    .print       = talaria_gpio_print,
    .make_new    = talaria_gpio_make_new,
    .locals_dict = (mp_obj_dict_t*)&talaria_gpio_locals_dict,
};

///////////////////////////////////////////////////////////////////////////////
// wifi class
///////////////////////////////////////////////////////////////////////////////
#if 0
typedef struct _talaria_wifi_obj_t {
    mp_obj_base_t base;
} talaria_wifi_obj_t;


extern const mp_obj_type_t talaria_wifi_type;

mp_obj_t
talaria_wifi_make_new( const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args )
{
    mp_arg_check_num(n_args, n_kw, 1, 1, true);

    talaria_wifi_obj_t *self = m_new_obj(talaria_wifi_obj_t);
    self->base.type  = &talaria_wifi_type;
    return MP_OBJ_FROM_PTR(self);
}

static void
talaria_wifi_print( const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind )
{
    talaria_wifi_obj_t *self = MP_OBJ_TO_PTR(self_in);

    mp_printf(print, "wifi");
}

static mp_obj_t
talaria_wifi_scan(mp_obj_t self_in)
{
    talaria_wifi_obj_t *self = MP_OBJ_TO_PTR(self_in);
    return mp_const_none;
}

static mp_obj_t
talaria_wifi_connect(mp_obj_t self_in)
{
    talaria_wifi_obj_t __unused *self = MP_OBJ_TO_PTR(self_in);

    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_1(talaria_wifi_scan_obj,    talaria_wifi_scan);
MP_DEFINE_CONST_FUN_OBJ_1(talaria_wifi_connect_obj, talaria_wifi_connect);

static const mp_rom_map_elem_t talaria_wifi_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_scan),    MP_ROM_PTR(&talaria_wifi_scan_obj) },
    { MP_ROM_QSTR(MP_QSTR_connect), MP_ROM_PTR(&talaria_wifi_connect_obj) },
};
static MP_DEFINE_CONST_DICT(talaria_wifi_locals_dict, talaria_wifi_locals_dict_table);

const mp_obj_type_t talaria_wifi_type = {
    { &mp_type_type },
    .name        = MP_QSTR_wifi,
    .print       = talaria_wifi_print,
    .make_new    = talaria_wifi_make_new,
    .locals_dict = (mp_obj_dict_t*)&talaria_wifi_locals_dict,
};
#endif
///////////////////////////////////////////////////////////////////////////////
// module talaria
///////////////////////////////////////////////////////////////////////////////

static const mp_map_elem_t talaria_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_talaria) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_gpio), (mp_obj_t)&talaria_gpio_type },
//    { MP_OBJ_NEW_QSTR(MP_QSTR_wifi), (mp_obj_t)&talaria_wifi_type },
};

static MP_DEFINE_CONST_DICT (mp_module_talaria_globals, talaria_globals_table);

const mp_obj_module_t mp_module_talaria = {
    .base    = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_talaria_globals,
};
