/*
 * scull.c
 *
 *  Created on: 13 juin 2013
 *      Author: telemaque
 */

#include <linux/module.h>
#include <linux/types.h>

#include "sleepy_dev.h"

MODULE_LICENSE("GPL");

#define DEVICE_NAME "sleepy"
#define DEVICE_COUNT 1

sleepy_dev_t * dev;

static int /*__init*/ sleepy_init(void)
{
	dev = sleepy_dev_construct(DEVICE_COUNT, DEVICE_NAME);
	if (!dev)
		return -1;

	printk(KERN_WARNING "sleepy init success \n");

	return 0;
}

static void sleepy_exit(void)
{
	printk(KERN_WARNING "sleepy exit\n");
	sleepy_dev_destroy(dev);
}

module_init(sleepy_init);
module_exit(sleepy_exit);
