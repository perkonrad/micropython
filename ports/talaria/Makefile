
FWTOP   = ../../../..
T2SDK   = $(FWTOP)/build
LIBDEST = $(FWTOP)/libs/arm

all : $(LIBDEST)/libpy.a $(LIBDEST)/libpym.a

include ../../py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# include py core make definitions
include $(TOP)/py/py.mk
include $(TOP)/py/mkrules.mk
include $(T2SDK)/sdk.mak

CFLAGS += -I$(TOP)/lib/libm/ -I${includedir} -I${includedir}/arm -I. -I$(TOP) -I$(BUILD)

#
# libpy
#
LIB_PY_OBJS  = $(addprefix $(TOP)/py/,	\
	mpstate.o			\
	nlr.o				\
	nlrx86.o			\
	nlrx64.o			\
	nlrthumb.o			\
	nlrxtensa.o			\
	nlrsetjmp.o			\
	malloc.o			\
	gc.o				\
	pystack.o			\
	qstr.o				\
	vstr.o				\
	mpprint.o			\
	unicode.o			\
	mpz.o				\
	reader.o			\
	lexer.o				\
	parse.o				\
	scope.o				\
	compile.o			\
	emitcommon.o			\
	emitbc.o			\
	asmbase.o			\
	asmx64.o			\
	emitnx64.o			\
	asmx86.o			\
	emitnx86.o			\
	asmthumb.o			\
	emitnthumb.o			\
	emitinlinethumb.o		\
	asmarm.o			\
	emitnarm.o			\
	asmxtensa.o			\
	emitnxtensa.o			\
	emitinlinextensa.o		\
	formatfloat.o			\
	parsenumbase.o			\
	parsenum.o			\
	emitglue.o			\
	persistentcode.o		\
	runtime.o			\
	runtime_utils.o			\
	scheduler.o			\
	nativeglue.o			\
	stackctrl.o			\
	argcheck.o			\
	warning.o			\
	map.o				\
	obj.o				\
	objarray.o			\
	objattrtuple.o			\
	objbool.o			\
	objboundmeth.o			\
	objcell.o			\
	objclosure.o			\
	objcomplex.o			\
	objdeque.o			\
	objdict.o			\
	objenumerate.o			\
	objexcept.o			\
	objfilter.o			\
	objfloat.o			\
	objfun.o			\
	objgenerator.o			\
	objgetitemiter.o		\
	objint.o			\
	objint_longlong.o		\
	objint_mpz.o			\
	objlist.o			\
	objmap.o			\
	objmodule.o			\
	objobject.o			\
	objpolyiter.o			\
	objproperty.o			\
	objnone.o			\
	objnamedtuple.o			\
	objrange.o			\
	objreversed.o			\
	objset.o			\
	objsingleton.o			\
	objslice.o			\
	objstr.o			\
	objstrunicode.o			\
	objstringio.o			\
	objtuple.o			\
	objtype.o			\
	objzip.o			\
	opmethods.o			\
	sequence.o			\
	stream.o			\
	binary.o			\
	builtinimport.o			\
	builtinevex.o			\
	builtinhelp.o			\
	modarray.o			\
	modbuiltins.o			\
	modcollections.o		\
	modgc.o				\
	modio.o				\
	modmath.o			\
	modcmath.o			\
	modmicropython.o		\
	modstruct.o			\
	modsys.o			\
	moduerrno.o			\
	modthread.o			\
	vm.o				\
	bc.o				\
	showbc.o			\
	repl.o				\
	smallint.o			\
	frozenmod.o			\
	)

LIB_PY_OBJS += $(TOP)/lib/utils/pyexec.o
LIB_PY_OBJS += $(TOP)/lib/mp-readline/readline.o
LIB_PY_OBJS += modtalaria.o

$(LIBDEST)/libpy.a : $(LIB_PY_OBJS)
	ar ru $@ $^
	ranlib $@

#
# libpym
#
LIB_PYM_OBJS  = $(addprefix $(TOP)/lib/libm/,	\
		math.o					\
		acoshf.o				\
		asinfacosf.o				\
		asinhf.o				\
		atan2f.o				\
		atanf.o					\
		atanhf.o				\
		ef_rem_pio2.o				\
		erf_lgamma.o				\
		fmodf.o					\
		kf_cos.o				\
		kf_rem_pio2.o				\
		kf_sin.o				\
		kf_tan.o				\
		log1pf.o				\
		nearbyintf.o				\
		sf_cos.o				\
		sf_erf.o				\
		sf_frexp.o				\
		sf_ldexp.o				\
		sf_modf.o				\
		sf_sin.o				\
		sf_tan.o				\
		wf_lgamma.o				\
		wf_tgamma.o				\
		ef_sqrt.o				\
		)

$(LIBDEST)/libpym.a : $(LIB_PYM_OBJS)
	ar ru $@ $^
	ranlib $@

$(LIB_PY_OBJS) $(LIB_PYM_OBJS) : $(BUILD)/genhdr/qstrdefs.generated.h $(BUILD)/genhdr/mpversion.h

#
# Auto dependencies
#
-include $(LIB_PY_OBJS:.o=.d) $(LIB_PYM_OBJS:.o=.d)

clean : clean-lib
clean-lib :
	rm -f $(LIBDEST)/libpy.a $(LIBDEST)/libpym.a $(LIB_PY_OBJS) $(LIB_PYM_OBJS) $(LIB_PYM_OBJS:.o=.d) $(LIB_PY_OBJS:.o=.d)

.PHONY : clean-lib
