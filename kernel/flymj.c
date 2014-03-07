#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include "../include/common.h"

MODULE_LICENSE("Dual BSD/GPL");

static int hello_open(struct inode *inode, struct file *file)
{
	printk(KERN_ALERT "device opened\n");
	return 0;
}
static int hello_release(struct inode *inode, struct file *file)
{
	printk(KERN_ALERT "device closed\n");
	return 0;
}

static void sleep_schedule(unsigned long argv)
{
	flymj_msleep_t *msleep = (flymj_msleep_t*)kmalloc(sizeof(flymj_msleep_t), __GFP_WAIT);
	unsigned long long timeout_jiffies; 
	printk(KERN_ALERT "start sleep_schedule\n");

	copy_from_user(msleep, (flymj_msleep_t*)argv, sizeof(flymj_msleep_t));
	timeout_jiffies = msleep->count * HZ /1000000;
	if( !timeout_jiffies )
	{
		timeout_jiffies  = 1;
	}
	printk(KERN_ALERT "sleep %lld jiffies\n", timeout_jiffies);
	
	timeout_jiffies += jiffies;
	while(time_before(jiffies, timeout_jiffies))
	{
		schedule();
	}

	kfree(msleep);

}
static void sleep_busy(unsigned long argv)
{
 	unsigned long long timeout_jiffies = 0;
	flymj_msleep_t *msleep = (flymj_msleep_t*)kmalloc(sizeof(flymj_msleep_t), __GFP_WAIT);
	
	copy_from_user(msleep, (flymj_msleep_t*)argv, sizeof(flymj_msleep_t));
	timeout_jiffies = msleep->count * HZ / 1000000;

	printk(KERN_ALERT "start sleep_busy\n");
	
	if( !timeout_jiffies )
	{
		timeout_jiffies = 1;
	}
	
	printk(KERN_ALERT "sleep %lld jiffies\n", timeout_jiffies);
	
	timeout_jiffies += jiffies;
	while(time_before(jiffies, timeout_jiffies))
	{
	}

	kfree(msleep);
}
static void sleep_sched_timeout(unsigned long argv)
{
 	unsigned long long timeout_jiffies = 0;
	flymj_msleep_t *msleep = (flymj_msleep_t*)kmalloc(sizeof(flymj_msleep_t), __GFP_WAIT);
	
	copy_from_user(msleep, (flymj_msleep_t*)argv, sizeof(flymj_msleep_t));
	timeout_jiffies = msleep->count * HZ / 1000000;

	printk(KERN_ALERT "start sleep_schedule_timeout\n");
	
	if( !timeout_jiffies )
	{
		timeout_jiffies = 1;
	}
	printk(KERN_ALERT "sleep %lld jiffies\n", timeout_jiffies);
	set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(timeout_jiffies);

	kfree(msleep);
}

static long  hello_ioctl(struct file *file, unsigned int cmd, unsigned long argv)
{
	printk(KERN_ALERT "ioctl with cmd = %d\n", cmd);
	switch(cmd)
	{
		case FLYMJ_IOCTL_SLEEP_WITH_SCHEDULE:
			printk(KERN_ALERT "we hit the sleep with schedule path\n");
			sleep_schedule(argv);
			break;
		case FLYMJ_IOCTL_SLEEP_WITH_SCHEDULE_TIMEOUT:
			printk(KERN_ALERT "we hit the sleep with schedule timeout path\n");
			sleep_sched_timeout(argv);
			break;
		case FLYMJ_IOCTL_SLEEP_WITH_BUSY_WAIT:
			printk(KERN_ALERT "we hit the sleep with schedule timeout path\n");
			sleep_busy(argv);
			break;
		default:
			break;
	}
	return 0;
}

int major_no;
#define DEVICE_NAME "flymj"
static struct class *hello_class;
static struct file_operations fops = 
{
	.open = hello_open,
	.release = hello_release,
	.unlocked_ioctl= hello_ioctl
};

static int hello_init(void)
{
    printk(KERN_ALERT "hello,I am edsionte/n");
	major_no =  register_chrdev(0, DEVICE_NAME, &fops);
	printk(KERN_ALERT "\nMajor number = %d\n", major_no);
	hello_class = class_create(THIS_MODULE, DEVICE_NAME);
	device_create(hello_class, NULL, MKDEV(major_no, 0), NULL, DEVICE_NAME);
	printk(KERN_ALERT "Device Initialized\n");
	printk(KERN_ALERT "hz = %d\n", HZ);
    return 0;
}

static void hello_exit(void)
{
	device_destroy(hello_class, MKDEV(major_no, 0));
	class_unregister(hello_class);
	class_destroy(hello_class);
	unregister_chrdev(major_no, DEVICE_NAME);
    printk(KERN_ALERT "goodbye,kernel/n");
}

module_init(hello_init);
module_exit(hello_exit);
