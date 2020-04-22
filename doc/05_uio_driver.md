[<< back](04_devmem.md) | [index](01_index.md) | [forward >> ](06_ioctl_driver.md)

# UIO Driver

## Overview

Writing a Linux driver always imposes the risk of crashing the whole kernel. Additionally drivers are hard to debug, so whenever possible, writing kernel code should be avoided.

In order to achieve this, Linux provides a generalized user-space I/O driver (UIO). The idea is to move the logic of the driver itself into the user space and only have a very simple kernel module that tells the generalized driver which addresses etc. to use.

## 1. Edit Device Tree

First we have to let linux know, that there is a UIO device and at which addresses it is. To do so, the device tree of the reference design must be edited.

Details about location of devicetree files, how to build them, etc. can be found in the  *[Enclustra Build Environment - HowTo Guide](https://download.enclustra.com/public_files/Design_Support/Application%20Notes/Enclustra_Build_Environment_HowToGuide_V02.pdf)[2]*

In our case, we add the section below:

<pre>
    uio_fpga_base@43C10000{	
		status = "okay";
		compatible = "generic-uio";
		reg = < 0x43C10000 0x1000 >;
	};
</pre>

The full device-tree file is available in [[root]/uio_driver/zx5-obru-uio.dts](../uio_driver/zx5-obru-uio.dts)

## 2. Compile Device Tree

The easiest way to compile the edited devicetree, is copying it to the folder *[root]/bsp-xilinx/sources/xilinx-linux/arch/arm/boot/dts* directory of your Enclustra Build Environment.

The device-tree can then be compiled into a devicetree-blob using the command below (from within the *dts* drectiory mentioned above):

<pre>
dtc -O dtb -o zx5-obru-uio.dtb zx5-obru-uio.dts
</pre>

The output file *zx5-obru-uio.dts* must be copied to the boot partition of the SD card and renamed to *devicetree.dtb* (as expected by the boot process) and hence replace de default *devicetree.dtb* file.

## 3. Write Kernel Module

The good thing about using UIO: you don't have to write any Kernel code. We will use the generic driver that is built-in. 

However, to do so, we need to configure the Kernel correctly.

## 4. Configure Kernel

The following configuration options are required in the Kernel to correctly run the generic UIO driver:

<pre>
	UIO_PDRV_GENIRQ=M
	UIO_DMEM_GENIRQ=M
</pre>

It is easiest to just open *menuconfig* and search for the configuration names using '/'. After configuring the kernel correctly, you can have to rebuild it. For details about kernel configuration and building the kernel,  see *[Enclustra Build Environment - HowTo Guide ](https://download.enclustra.com/public_files/Design_Support/Application%20Notes/Enclustra_Build_Environment_HowToGuide_V02.pdf)[2]*. 

Now copy the resulting *uImage* (*[root]/bsp-xilinx/sources/xilinx-linux/arch/arm/boot/uImage*) to the *boot* partition of your SD card.

Note that it is required to build the UIO drivers mentioned above as module (M) and not as built-in (*). The reason is, that probe must be called manually with additional parameters to use them. If they are built-in, this is not possible.

## 5. Installing Kernel Modules

Because of the changed configuration, the kernel does no more contain the generic UIO driver as built-in (default), so we have to provide it in *rootfs*. To do so, follow the steps below:

Most likely you still have your console open from step 4. If not, open one, set all environment vairables as required for building the kernel (see *[Enclustra Build Environment - HowTo Guide ](https://download.enclustra.com/public_files/Design_Support/Application%20Notes/Enclustra_Build_Environment_HowToGuide_V02.pdf)[2]*) and navigate to the linux build directory (*[root]/bsp-xilinx/sources/xilinx-linux*).

Now, the optional modules must be built:;

<pre>
    make modules
</pre>

To install the modules on your SD Card (*rootfs* partition), execute the command below:

<pre>
    make modules_install INSTALL_MOD_PATH=<i>[path-to-your-sd-card-rootfs-partition]</i>
</pre>

In my case this is:

<pre>
    make modules_install INSTALL_MOD_PATH=/media/obruendl/rootfs
</pre>

## 6. Loading Kernel Module

Follow the steps below to load the kernel module:

Boot the target device.

Load the generic UIO driver:

<pre>
    modprobe uio_pdrv_genirq of_id="generic-uio"
</pre>

The *of_id="generic_uio"* parameter configures the driver to be loaded for all devicetree entries with this compatible string (like the one we created).


You should now see a new device named *uio0* popping up in the */dev* folder. Note that UIO devices do not have a readable name but they are numbered starting at zero. This is not very handy. However, there are ways to acquire more information. These are described below.

## 7. Getting Information about a UIO Device

The plain numbering of UIO devices without a human readable name can easily lead to confusion. Luckily, there are some ways to find out more about a UIO device.

One way to learn more about UIO devices is suing the */sys/class/uio* directory structure. For example:

<pre>
    # cat /sys/class/uio/uio0/name
    uio_fpga_base
</pre>


Another even more handy way, is the *lsuio* utility. It is not enabled by default but it can easily be enabled when configuring buildroot. Just search (using '/') for "lsuio" and enable the corresponding package. If *lsuio* is present you can use it as described below:

<pre>
    # lsuio
    uio0: name=uio_fpga_base, version=devicetree, events=0
            mape[0]: addr=0x43C10000, size=4096
</pre>


## 8. Writing a User Space Application

A small user space application is provided along with the example in order to show how to use the UIO driver from user space. The application is provided as *Xilinx SDK* project. The source file can be found in [[root]/uio_driver/app/src/helloworld.c](../uio_driver/app/src/helloworld.c).

The program only has a hand full of lines and explanatory comments. So it is not described here in more detail. Just have a look at the sources.

To build the application, follow the steps below:

1. Start SDK
2. Create/choose an empty workspace
3. Click *File > Import*
4. Choose *General > Existing Projects into Workspace*
5. Select the folder *[root]/uio_driver*
6. Press *Finish*

In SDK you should now see a project called *uio_test*. By default SDK should automatically build the project and produce a *.elf file. If you disabled automatic build in SDK, you have to manually build the project using *Project > Build All*.

![sdk_project.png](./05_pics/sdk_project.png)

Now copy the *uio_test.elf*  file to the directory */root/uio_driver* of your SD Card (*rootfs* partition).

## 9. Test User Space Application

Follow the steps below to load the kernel module:

Boot the target device.

Then navigate to the correct directory on your *rootfs* ...

<pre>
    cd /root/uio_driver
</pre>

... and start the application. Before, the UIO driver must be loaded of course.

<pre>
    modprobe uio_pdrv_genirq of_id="generic_uio"
    ./uio_test.elf
</pre>


You should now see the following output:

<pre>
    Hello World
    version=0xAB12CD34
    year=2020
    sw-version=0x0000ABCD
</pre>

Note that the "year" output may change according to the year you built the FPGA bitstream in.

## 10. Conclusion

In this chapter, a simple UIO based device driver was utilized. It was used from a very simple application. To achieve this, we did not have to write a single line of kernel code.

Of course only the very simplest case of a driver is covered, but this should be a good starting point to base your own development of a real (and more complex) driver on.



[<< back](04_devmem.md) | [index](01_index.md) | [forward >> ](06_ioctl_driver.md)
