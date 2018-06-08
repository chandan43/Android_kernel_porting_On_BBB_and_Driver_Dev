#ARCH ?= $(shell uname -m)
#OS ?= $(shell uname -s)
#CFLAGS +=
obj-m := led.o
obj-m += mydummy.o
CFLAGS_led.o := -fno-pic
CFLAGS_mydummy.o := -fno-pic

KDIR =  /root/bbbandroid/kernel
CROSS_COMPILE ?= arm-linux-androideabi-
ARCH = arm

PWD := $(shell pwd)

default:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) SUBDIRS=$(PWD) modules 

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean

#make ARCH=arm CROSS_COMPILE=arm-linux-androideabi- CFLAGS_MODULE=-fno-pic
