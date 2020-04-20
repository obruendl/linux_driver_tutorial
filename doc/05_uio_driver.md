[<< back](04_devmem.md) | [index](01_index.md) | [forward >> ](04_uio_driver.md)

# UIO Driver

## Overview

Writing a Linux driver always imposes the risk of crashing the whole kernel. Additionally drivers are hard to debug, so whenever possible, it should be avoided writing kernel code.

In order to achieve this, Linux provides a generalized user-space I/O driver (UIO). The idea is to move the logic of the driver itself into the user space and only have a very simple kernel module that tells the generalized driver which addresses etc. to use.

## 1. Edit Device Tree

First we have to let linux know, that there is a UIO device and at which addresses it is. To do so, the device tree of the reference design must be edited.

Details about location of devicetree files, how to build them, etc. can be found in the  [Enclustra Build Environment - HowTo Guide](https://download.enclustra.com/public_files/Design_Support/Application%20Notes/Enclustra_Build_Environment_HowToGuide_V02.pdf)

In our case, we add the section below:

<pre>
    uio_fpga_base@43C10000{	
		status = "okay";
		compatible = "generic-uio";
		reg = < 0x43C10000 0x1000 >;
	};
</pre>

The full device-tree file is available in [[root]/uio_driver/zx5-obru-uio.dts](../uio_driver/zx5-obru-uio.dts)

## 3. Compile Device Tree

The easiest way to compile the edited devicetree, is copying it to the folder *[root]/bsp-xilinx/sources/xilinx-linux/arch/arm/boot/dts* directory of your Enclustra Build Environment.

The device-tree can then be compiled into a devicetree-blob using the command below (from within the *dts* drectiory mentioned above):

<pre>
dtc -O dtb -o zx5-obru-uio.dtb zx5-obru-uio.dts
</pre>

The output file *zx5-obru-uio.dts* must be copied to the boot partition of the SD card and renamed to *devicetree.dtb* (as expected by the boot process).

## 4. Write Kernel Module

Bla, bla, bla

## 5. Load Kernel Module

Bla, bla, bla

## 6. Write User Space Application

Bla bla bla

## Test User Space Application

Bla bla bla



[<< back](04_devmem.md) | [index](01_index.md) | [forward >> ](04_uio_driver.md)