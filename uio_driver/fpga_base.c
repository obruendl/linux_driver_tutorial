#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oliver Bruendler");
MODULE_DESCRIPTION("fpga_base driver");

static struct uio_info info = {
	.name = "fpga_base",
	.version = "1.0",
	.irq = UIO_IRQ_NONE,
};

static struct resource uio_resources[] = {
	[0] = {
		.name	= "reg",
		.start	= 0x43C10000,
		.end	= 0x43C10FFF,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.name	= "dummy",
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device my_dev = {
	.name = "uio_pdrv_genirq",
	.id = -1,
	.dev = {
		.platform_data = &info,
	},
	.resource	= uio_resources,
	.num_resources	= ARRAY_SIZE(uio_resources),
};

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
