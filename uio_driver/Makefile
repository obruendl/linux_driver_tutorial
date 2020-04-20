MODULES := fpga_base.o

ARCH := arm

CROSS_COMPILE := arm-none-linux-gnueabi-

obj-m := $(MODULES)

#Kernel-Sources (this path must be changed on your machine!)
ROOTDIR := /home/obruendl/Ebe/bsp-xilinx/sources/xilinx-linux

MAKEARCH := $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)

all: modules

modules:
	$(MAKEARCH) -C $(ROOTDIR) M=${shell pwd} modules

clean: 
	$(MAKEARCH) -C $(ROOTDIR) M=${shell pwd} clean
