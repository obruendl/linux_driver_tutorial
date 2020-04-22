MODULES := fpga_base_ioctl_irq.o

ARCH := arm

CROSS_COMPILE := arm-none-linux-gnueabi-

obj-m := $(MODULES)

ROOTDIR := $(BSP_XILINX)/sources/xilinx-linux
MAKEARCH := $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)

all: modules

modules:
	$(MAKEARCH) -C $(ROOTDIR) M=${shell pwd} modules

clean: 
	$(MAKEARCH) -C $(ROOTDIR) M=${shell pwd} clean
