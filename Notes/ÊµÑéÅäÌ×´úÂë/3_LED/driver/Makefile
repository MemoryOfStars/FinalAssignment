# Makefile  
# Please modify here or set environments.
# KSOURCE should be pointed to the build directory of your kernel.
#
DEBUG ?= n
KSOURCE ?= /usr/src/linux-2.6.32.2

%.x:%.c
	arm-linux-gcc -o $@ $<

KBUILD_VERBOSE:=2

obj-m := mini2440_leds.o

default:
	make -C $(KSOURCE) LANG=C KBUILD_VERBOSE=${KBUILD_VERBOSE}  SUBDIRS=`pwd` modules

.PHONY: cscope
cscope:
	cscope -b -k -R

.PHONY: clean
clean:
	make -C $(KSOURCE) LANG=C KBUILD_VERBOSE=${KBUILD_VERBOSE}  SUBDIRS=`pwd` clean
	rm -f *.x *~
