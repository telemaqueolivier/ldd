/*
 * sleepy_dev.c
 *
 *  Created on: 30 oct. 2014
 *      Author: telemaque
 */

#include <linux/slab.h>
#include <linux/fs.h>
#include "sleepy_dev.h"
#include <linux/string.h>
#include "file_operations.h"

struct sleepy_dev
{
	char name[256];
	unsigned int major;
	struct file_operations fops;
	wait_queue_head_t wait_queue;
	int flag;
};

sleepy_dev_t * sleepy_dev_construct(unsigned int num, const char * name)
{
	sleepy_dev_t * dev = kmalloc(sizeof(sleepy_dev_t), GFP_KERNEL);
	if (!dev)
		return NULL;

	init_waitqueue_head(&dev->wait_queue);

	strcpy(dev->name, name);

	dev->fops.owner = THIS_MODULE;
	dev->fops.read = sleepy_read;
	dev->fops.write = sleepy_write;

	dev->flag = 0;

	dev->major = register_chrdev(0, dev->name, &dev->fops);
	if(dev->major < 0)
		goto out_1;

	return dev;

	out_1:
	kfree(dev);
	return NULL;
}

void sleepy_dev_destroy(sleepy_dev_t * dev)
{
	unregister_chrdev(dev->major, dev->name);
	kfree(dev);
}

wait_queue_head_t * wait_queue(sleepy_dev_t * dev)
{
	return &dev->wait_queue;
}

int flag(sleepy_dev_t * dev)
{
	return dev->flag;
}

void set_flag(sleepy_dev_t * dev, int value)
{
	dev->flag = value;
}
