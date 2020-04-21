#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <asm/io.h>


//*************************************************************************
//******************************** Definitions ****************************
//*************************************************************************

#define DEVICE_NAME "dummy"
#define MAJOR_NUM 42

typedef struct date_s {
	int year;
	int month;
	int day;
	int hour;
	int minute;
} date_t;

typedef struct prvivate_s {
	void* regs;
	struct platform_device* pdev;
	struct miscdevice mdev;
} private_t;

#define WR_SW_VERSION _IOW('a','a',uint32_t)
#define RD_SW_VERSION _IOR('a','b',uint32_t*)
#define RD_FW_VERSION _IOR('a','c',uint32_t*)
#define RD_DATE _IOR('a','d', date_t*)



//*************************************************************************
//***************************** Implementation ****************************
//*************************************************************************

static inline private_t* to_private_t(struct file *file)
{
	struct miscdevice *miscdev = file->private_data;
	return container_of(miscdev, private_t, mdev);
}


//static struct class *dummy_class;

static int fpga_base_open(struct inode *inode, struct file *file)
{
	printk("fpga_base open\n");
	return 0;
}

static int fpga_base_release(struct inode *inode, struct file *file)
{
	printk("fpga_base release\n");
	return 0;
}

static ssize_t fpga_base_read(struct file *file,
			char *buffer, size_t length, loff_t * offset)
{
	printk("fpga_base read\n");
	return 0;
}

static ssize_t fpga_base_write(struct file *file,
			 const char *buffer, size_t length, loff_t * offset)
{
	printk("fpga_base write\n");
	return length;
}

static long fpga_base_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret;
	void* regs;
	private_t* priv;
	date_t date;
	printk("fpga_base ioctl\n");
	priv = to_private_t(file);
	regs = (uint32_t*)priv->regs;
	
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
 

struct file_operations fpga_base_fops = {
	.owner = THIS_MODULE,
	.open = fpga_base_open,
	.release = fpga_base_release,
	.read = fpga_base_read,
	.write = fpga_base_write,
	.unlocked_ioctl = fpga_base_ioctl,
};

//*************************************************************************
//***************************** Initialization ****************************
//*************************************************************************
static int fpga_base_probe(struct platform_device *pdev)
{
	private_t* priv;
	int ret;
	struct resource* res;

	priv = kmalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;
	
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	priv->regs = ioremap_nocache(res->start, resource_size(res));

	platform_set_drvdata(pdev, priv);
	priv->pdev = pdev;

	priv->mdev.minor 	= 0;
	priv->mdev.name 	= "fpga_base";
	priv->mdev.fops		= &fpga_base_fops;
	priv->mdev.parent	= NULL;

	ret = misc_register(&priv->mdev);
	if (ret) {
		printk("Failed to register miscdev in fpga_base\n");
		return ret;
	}

	printk("Registered miscdev in fpga_base\n");
	return 0;
}

static int fpga_base_remove(struct platform_device *pdev)
{
	private_t* priv = platform_get_drvdata(pdev);

	misc_deregister(&priv->mdev);
	kfree(priv);
	printk("Unregistered miscdev in fpga_base\n");

	return 0;
}

static const struct of_device_id fpga_base_match[] = {
	{	.compatible = "fpga-base",	},
	{},
};

static struct platform_driver fpga_base_driver = {
	.probe	= fpga_base_probe,
	.remove	= fpga_base_remove,
	.driver = {
		.name = "fpga-base",
		.of_match_table = of_match_ptr(fpga_base_match),
	},
};

int __init fpga_base_init(void)
{
	return platform_driver_register(&fpga_base_driver);
}

void __exit fpga_base_exit(void)
{
	platform_driver_unregister(&fpga_base_driver);
}


module_platform_driver(fpga_base_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oliver Bründler");
MODULE_DESCRIPTION("A simple device driver");
MODULE_VERSION("1.0");
	


/*void __exit dummy_exit(void)
{
	int i;
	i = 0;
	device_destroy(dummy_class, MKDEV(MAJOR_NUM, i));
	class_destroy(dummy_class);

	unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
	printk("Dummy unloaded\n");
}*/



//module_init(fpga_base_init);
//module_exit(dummy_exit);




 

/* 
dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
 */

 /* 
static int __init etx_driver_init(void)
{
        //Allocating Major number
        if((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) <0){
                printk(KERN_INFO "Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        //Creating cdev structure
        cdev_init(&etx_cdev,&fops);
 
        //Adding character device to the system
        if((cdev_add(&etx_cdev,dev,1)) < 0){
            printk(KERN_INFO "Cannot add the device to the system\n");
            goto r_class;
        }
 
        //Creating struct class
        if((dev_class = class_create(THIS_MODULE,"etx_class")) == NULL){
            printk(KERN_INFO "Cannot create the struct class\n");
            goto r_class;
        }
 
        //Creating device
        if((device_create(dev_class,NULL,dev,NULL,"etx_device")) == NULL){
            printk(KERN_INFO "Cannot create the Device 1\n");
            goto r_device;
        }
        printk(KERN_INFO "Device Driver Insert...Done!!!\n");
    return 0;
 
r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}*/
 
/*void __exit etx_driver_exit(void)
{
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&etx_cdev);
        unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}*/
 

 

