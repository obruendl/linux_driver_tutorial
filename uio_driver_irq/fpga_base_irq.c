//***************************************************************************************
// Includes
//***************************************************************************************
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h>

//***************************************************************************************
// Module Information
//***************************************************************************************
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oliver Bruendler");
MODULE_DESCRIPTION("fpga_base driver");


//***************************************************************************************
// Implementation
//***************************************************************************************
static struct resource uio_resources[] = {
	[0] = {
		.name	= "reg",			//Registers
		.start	= 0x43C10000,		//Start address
		.end	= 0x43C10FFF,		//End address
		.flags	= IORESOURCE_MEM,	//Registers are threated as IO memory
	},
	[1] = {
		.name	= "dummy",			//We do not use an IRQ, so let's name th resource "dummy" (it is required, so we cannot omit it)
		.flags	= IORESOURCE_IRQ,	//This is an IRQ
	},
};

//Information about our UIO module
static struct uio_info info = {
	.name = "fpga_base",
	.version = "1.0",
	.irq = UIO_IRQ_NONE,	//We do not handle an IRQ
};

//Information for registering our driver
static struct platform_device my_dev = {
	.name = "uio_pdrv_genirq",	//Name of the general uio driver 
	.id = -1,					//There is only one uio_pdrv_genericirq device (generic driver object)
	.dev = {
		.platform_data = &info,
	},
	.resource	= uio_resources,
	.num_resources	= ARRAY_SIZE(uio_resources),
};

//***************************************************************************************
// Handling of module insertaion / removal
//***************************************************************************************
static int __init uio_init(void)
{
	printk(KERN_INFO "fpga_base uio_init()\n");
	return platform_device_register(&my_dev);
}

static void __exit uio_cleanup(void)
{
	printk(KERN_INFO "fpga_base uio_cleanup()\n");
	platform_device_unregister(&my_dev);
}


module_init(uio_init);
module_exit(uio_cleanup);
