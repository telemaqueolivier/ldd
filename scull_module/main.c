/*
 * scull.c
 *
 *  Created on: 13 juin 2013
 *      Author: telemaque
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <asm-generic/uaccess.h>
#include "scull_dev.h"
MODULE_LICENSE("GPL");

#define DEVICE_NAME "scull"
#define NUM_DEVICES 1

static unsigned int num_devices;
static unsigned int list_size;
static unsigned int qset_size;
static unsigned int quantum_size;
static scull_dev *dev;

module_param(num_devices, uint, S_IRUGO);
module_param(list_size, uint, S_IRUGO);
module_param(qset_size, uint, S_IRUGO);
module_param(quantum_size, uint, S_IRUGO);

static int /*__init*/ scull_init(void)
{
	dev_t dev_num;
	int ret = alloc_chrdev_region(&dev_num, 0, num_devices, DEVICE_NAME);

	if(ret != 0) {
		printk(KERN_WARNING "can't allocate major number\n");
		return -1;
	}

	dev = scull_construct(0, dev_num, list_size, qset_size, quantum_size);
	if(!dev) {
		printk(KERN_WARNING "can't setup char device\n");
		return -1;
	}

	printk(KERN_WARNING "scull init success \n");

	return 0;
}

static void scull_exit(void)
{
	int dev_num = dev->dev_num;

	printk(KERN_ALERT "scull exit\n");
	scull_destroy(dev);
	unregister_chrdev_region(dev_num, num_devices);
}

module_init(scull_init);
module_exit(scull_exit);
