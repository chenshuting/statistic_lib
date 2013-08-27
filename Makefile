# Comment/uncomment the following line to disable/enable debugging
#DEBUG = y

UNAME_R :=$(shell uname -r)

# Add your debugging flag (or not) to CFLAGS
ifeq ($(DEBUG),y)
  DEBFLAGS = -O -g -DYPDISK_DEBUG # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

EXTRA_CFLAGS += $(DEBFLAGS)
EXTRA_CFLAGS += -I..

ifneq ($(KERNELRELEASE),)
	obj-m := statistic_lib.o
else
        KERNELDIR ?= /lib/modules/$(shell uname -r)/build
        PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules -Wall
endif
install:
	install statistic_lib.ko ${DESTDIR}/lib/modules/$(UNAME_R)/kernel/drivers/block	

	@echo '==================================================================='
	@echo 'msios make-install done!'
	@echo '==================================================================='	

clean:
	rm -f *.o *.order Module.* *ko* *mod*  
