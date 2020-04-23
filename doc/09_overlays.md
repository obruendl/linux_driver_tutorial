[<< back](08_ioctl_irq.md) | [index](01_index.md) | [forward >> ](99_references.md)

# Device Tree Overlays

## Overview

Up to now, we always provided a device tree containing all our FPGA IP. This has a few drawbacks:

* It does not work if bitstreams are loaded after Linux is running (i.e. if scripts/applications running on Linux are configuring the FPGA)
* The main device tree of the boot partition must be modified every time a new IP is added.
* The device tree is not modular. If the same device-tree entries are required for different target devices, a separate device tree must be maintained for each one.

This problems can be addressed by using device tree overlays. Overlays allow to modify the device tree at runtime. So the standard device tree provided with the module can be used and specifics about the FPGA IP-Cores can be loaded and changed at runtime.

**Explanations in this section do not start from scratch but based on the state after the [UIO Driver Tutorial](./05_uio_driver).**

## 1. Compile Default Device Tree

As mentioned above, the default device tree shall be used. In order to allow overlays, thee default device tree must be compiled with symbols. Lets do this.

First, navigate to the folder *[root]/bsp-xilinx/sources/xilinx-linux/arch/arm/boot/dts* directory of your Enclustra Build Environment.

The device-tree can then be compiled into a devicetree-blob including symbols (*-@* option) using the command below (from within the *dts* drectiory mentioned above):

<pre>
dtc -@ -O dtb -o zynq-mercury-zx5.dtb zynq-mercury-zx5.dts
</pre>


The output file *zynq-mercury-zx5.dtb* must be copied to the boot partition of the SD card and renamed to *devicetree.dtb*. This file can now stay there and does not have to be changed during the project, because we can add our IP to the device tree using overlays.

## 2. Create Overlay

The overlay device tree source file is not described here in detail. Instead, explanatory comments are added to the source file. It makes sense that you read through the source file to understand the overlay syntax.

The overlay is described in the file [[root]/overlays/zx5-obru-uio-overlay.dts](../overlays/zx5-obru-uio-overlay.dts)

## 3. Compile Overlay

The easiest way to compile the edited devicetree, is copying it to the folder *[root]/bsp-xilinx/sources/xilinx-linux/arch/arm/boot/dts* directory of your Enclustra Build Environment.

The device-tree can then be compiled into a devicetree-blob-overlay (*.dtbo*) using the command below (from within the *dts* drectiory mentioned above):

<pre>
dtc -@ -O dtb -o zx5-obru-uio-overlay.dtbo zx5-obru-uio-overlay.dts
</pre>

Note that again the *-@* option is required to allow symbol links to other devcice tree blobs.

The output file *zx5-obru-uio-overlay.dtbo* must be copied to the *rootfs* partition of the SD card. In our example, we copy it to the directory */root/overlays*.

## 4. Configure Kernel

Enclustra EBE by default allows overlays. If you need to set the configuration manually, use *menuconfig* to search for the config. The required setting is

<pre>
    OF_OVERLAY=y
</pre>

 Because this option is enabled by default in Enclustra EBE, we don't have to recompile and install the kernel.

## 5. Testing the Overlay

First boot the target device.

Now load the UIO driver:

<pre>
   # modprobe uio_pdrv_genirq of_id="generic-uio"
</pre>

Because there is no *generic-uio* device in the device tree, no UIO device is loaded:

<pre>
	# ls /sys/class/uio
    #
</pre>

We will now load the overlay file we created. First we need to mount the config filesystem:

<pre>
    # mount -t configfs none /sys/kernel/config
</pre>

Then we need to create a directory for our overlay (one directory per overlay is required):

<pre>
    # mkdir -p /sys/kernel/config/device-tree/overlays/test
</pre>

To install the overlay, we simply write the contents of the *.dtbo* file to the correct file in the config filesystem:

<pre>
   # cat /root/overlay/zx5-obru-uio-overlay.dtbo > /sys/kernel/config/device-tree/test/dtbo
</pre>

The last three commands (from *mount* to *cat*) are also available as script in [[root]/overlays/install_overlay.sh](../overlays/install_overlay.sh).

Because the UIO driver is already loaded, our device gets recognized automatically:

<pre>
    # ls /sys/class/uio
    uio0
</pre>

From here on, we can interact with the device exactly the same way as it was described in the [UIO Driver Tutorial](./05_uio_driver).

The overlay can also be unloaded (e.g. when a different bitstream is loaded to the FPGA):

<pre>
    # rmdir /sys/kernel/config/device-tree/overlays/test
</pre>

After unloading the overlay, the UIO device is of course gone:

<pre>
	# ls /sys/class/uio
    #
</pre>

## 6. Conclusion

Device tree overlays allow adding devices to the device tree at runtime. This is very useful if bitstreams are exchanged at runtime. 

The mechanism can also be used, to always have the same device tree file to boot the system with and add all custom PL peripherals through overlay files (e.g. from a network drive). 



[<< back](08_ioctl_irq.md) | [index](01_index.md) | [forward >> ](99_references.md)