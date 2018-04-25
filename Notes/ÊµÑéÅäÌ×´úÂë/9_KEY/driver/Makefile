#
# Description:
# Makefile for 2.6 device drivers
#

# Comment/uncomment the following line to disable/enable debugging
#DEBUG = y

# Add your debugging flag (or not) to CFLAGS
ifeq ($(DEBUG),y)
DEBFLAGS = -O -g -DCAN_DEBUG
# "-O" is needed to expand inlines
else
DEBFLAGS = -O2
endif

#CFLAGS += $(DEBFLAGS) //i am not sure how to use this line

ifneq ($(KERNELRELEASE),)
# call from kernel build system

obj-m := onekey.o 

else

KERNELDIR ?= /usr/src/linux-2.6.32.2
PWD := $(shell pwd)

CC := arm-linux-gcc
HOSTCC := gcc

export CC HOSTCC

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

endif

clean:
	rm -rf *.o *.ko

