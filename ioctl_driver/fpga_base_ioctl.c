//*************************************************************************
// Includes
//*************************************************************************
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <asm/io.h>
#include "fpga_base_ioctl.h"

//*************************************************************************
// Helper Functions
//*************************************************************************

//Helper function to get private instance data from file pointer
static inline private_t* to_private_t(struct file *file)
{
	struct miscdevice *miscdev = file->private_data;	//file->private data is set to miscdevice data when miscdevice is opened the first time
	return container_of(miscdev, private_t, mdev);		//Because miscdevidce is part of the instance private data, we can access the instance private data using "container_of" (
}

//We do not have to do anything on open
static int fpga_base_open(struct inode *inode, struct file *file)
{
	printk("fpga_base open\n");
	return 0;
}

//We do not have to do anything on close
static int fpga_base_release(struct inode *inode, struct file *file)
{
	printk("fpga_base release\n");
	return 0;
}

//We do not use the read operation
static ssize_t fpga_base_read(struct file *file, char *buffer, size_t length, loff_t * offset)
{
	printk("fpga_base read\n");
	return 0;
}

//We do not use the write operation
static ssize_t fpga_base_write(struct file *file,
			 const char *buffer, size_t length, loff_t * offset)
{
	printk("fpga_base write\n");
	return length;
}

//IOCTLs we use!
static long fpga_base_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	//Declarations
	int ret;
	void* regs;
	private_t* priv;
	date_t date;
	
	//Initial print for information
	printk("fpga_base ioctl\n");
	
	//Get register pointer 
	priv = to_private_t(file);
	regs = (uint8_t*)priv->regs;
	
	//Implementation of operations
	//.. We implement them inline. For more complex operations, it is common to dispatch them to serparate functions
	//.. writel/readl are used to access registers. Data is transferred from/to userspace using put_user/get_user (single values)
	//.. and copy_to_user/copy_from_user for arrays and data structures.
	switch(cmd) {
		case WR_SW_VERSION:
			writel((uint32_t) arg, regs+0x18);
			printk(KERN_INFO "Sw Version = %d\n", (uint32_t) arg);
			break;
		case RD_SW_VERSION:
			put_user(readl(regs+0x18), ((uint32_t*)arg));
			break;
		case RD_FW_VERSION:
			put_user(readl(regs+0x00), ((uint32_t*)arg));
			break;
		case RD_DATE:
			date.year = readl(regs+0x04);
			date.month = readl(regs+0x08);
			date.day = readl(regs+0x0C);
			date.hour = readl(regs+0x10);
			date.minute = readl(regs+0x14);
			ret = copy_to_user((date_t*) arg, &date, sizeof(date));
			if (ret) {
				printk("Copy to user failed");
			}
			break;
		default:
			break;
        }
        return 0;
}
 
 //Here we define the functions t ocall for all file operations
struct file_operations fpga_base_fops = {
	.owner = THIS_MODULE,
	.open = fpga_base_open,
	.release = fpga_base_release,
	.read = fpga_base_read,
	.write = fpga_base_write,
	.unlocked_ioctl = fpga_base_ioctl,
};

//*************************************************************************
// Initialization
//*************************************************************************

//The probe function is called for each device (for each driver instance)
static int fpga_base_probe(struct platform_device *pdev)
{
	//Declarations
	private_t* priv;
	int ret;
	struct resource* res;
	struct device_node* np;
	void* ptr;

	//Kernel memory must be allocated using kmalloc
	priv = kmalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;
	
	//Read memory range from devicetree
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	//Physical memory (registers) must be mapped. For register, uncached mapping is required
	priv->regs = ioremap_nocache(res->start, resource_size(res));
	
	//Get a custom property from the devicetree
	np = pdev->dev.of_node;
	ptr = of_get_property(np, "somevalue", NULL);
	if (!ptr) {
		printk("Failed to read custom property in fpga_base\n");
		return ret;
	}
	printk("Somevalue read from dt = %08X\n", be32_to_cpup(ptr)); //Endian-correct reading of 32bits 

	//Setup driver private data.
	platform_set_drvdata(pdev, priv);
	priv->pdev = pdev;
	priv->mdev.minor 	= 0;
	priv->mdev.name 	= "fpga_base";	//This is the name being used in the /dev folder.
	priv->mdev.fops		= &fpga_base_fops;
	priv->mdev.parent	= NULL;

	//Register misc device
	ret = misc_register(&priv->mdev);
	if (ret) {
		printk("Failed to register miscdev in fpga_base\n");
		return ret;
	}

	//We are done
	printk("Registered miscdev in fpga_base\n");
	return 0;
}

//The remove function is called when the driver is unloaded
static int fpga_base_remove(struct platform_device *pdev)
{
	private_t* priv = platform_get_drvdata(pdev);
	
	//Unregister the device
	misc_deregister(&priv->mdev);
	
	//Free memory allocated in probe
	kfree(priv);
	
	//We are done
	printk("Unregistered miscdev in fpga_base\n");
	return 0;
}

//Here we define what HW (devicetree node) our driver can handle
static const struct of_device_id fpga_base_match[] = {
	{	.compatible = "psi,fpga-base",	},	//String must match devcietree "comatible" entry
	{},
};

//Functions to register/remove a platform driver
static struct platform_driver fpga_base_driver = {
	.probe	= fpga_base_probe,
	.remove	= fpga_base_remove,
	.driver = {
		.name = "fpga-base-driver",
		.of_match_table = of_match_ptr(fpga_base_match),
	},
};

//This function is called when the module is loaded (insmod)
int __init fpga_base_init(void)
{
	//Registration
	return platform_driver_register(&fpga_base_driver);
}

//This function is called when the modujle is unloaded (rmmod)
void __exit fpga_base_exit(void)
{
	//Deregistration
	platform_driver_unregister(&fpga_base_driver);
}

//Registe platform driver
module_platform_driver(fpga_base_driver);

//Module information
MODULE_LICENSE("GPLv2");
MODULE_AUTHOR("Oliver Bründler");
MODULE_DESCRIPTION("A simple device driver");
MODULE_VERSION("1.0");





 

