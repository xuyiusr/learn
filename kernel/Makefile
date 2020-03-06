#Makefile

TARGET:=hook_test

# If KERNELRELEASE is defined, we've been invoked from the  
# kernel build system and can use its language.  
ifneq ($(KERNELRELEASE),)
	obj-m := $(TARGET).o
	
# Otherwise we were called directly from the command  
# line; invoke the kernel build system. 
else
	KERNEL := /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNEL) M=$(PWD) modules

.PHONEY:clean
clean:
	@rm -f modules.order Module.symvers \
		*.o \
		*.mod.c *.ko
endif

