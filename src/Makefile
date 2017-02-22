#ifneq ($(KERNELRELEASE),)
# call from kernel build system


DRVNAME = ietest
obj-m += $(DRVNAME).o
$(DRVNAME)-objs += inputeventtest.o

#else

PWD       	= $(shell pwd)
#CROSS_COMPILE	?= arm-linux-gnueabi-
#KERNEL_PATH	?= /work/linux-3.14.41-ti-r63
KERNEL_PATH	?= /lib/modules/`uname -r`/build
ARCH		= x86
CROSS_COMPILE	=
#ARCH		?= arm
#export ARCH CROSS_COMPILE
#CROSS_COMPILE	?= gcc 


KBUILD_FLAGS 	= -W -Wall -Wstrict-prototypes -Wmissing-prototypes -C $(KERNEL_PATH) M=$(PWD)

default: modules

modules:
	echo $(MAKE) $(KBUILD_FLAGS) modules
	$(MAKE) $(KBUILD_FLAGS) modules
	@cp -f $(DRVNAME).ko ../bin/

clean:
	@clear
	@echo "Cleaning...."
	rm -rf *.o *~ *.ko *.mod.c .*.cmd .tmp* modules.order Module.symvers
	rm -f ../bin/$(DRVNAME).ko

clena: clean
celan: clean
celna: clean

#endif

