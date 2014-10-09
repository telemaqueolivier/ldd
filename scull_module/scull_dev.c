/*
 * scull_dev.c
 *
 *  Created on: 21 août 2013
 *      Author: telemaque
 */
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include "scull_dev.h"
#include "file_operations.h"

scull_dev * scull_construct(int index,
							dev_t dev_num,
							unsigned int list_size,
							unsigned int qset_size,
							unsigned int quantum_size)
{
	static struct file_operations fops = {
		.owner = THIS_MODULE,
		.llseek = scull_llseek,
		.read = scull_read,
		.write = scull_write,
		.unlocked_ioctl = scull_ioctl,
		.open = scull_open,
		.release = scull_release
	};
	int err;
	scull_dev *dev = kmalloc(sizeof(scull_dev), GFP_KERNEL);

	if (!dev) {
		printk(KERN_NOTICE "can't allocate scull device!");
		return NULL;
	}

	cdev_init(&dev->cdev, &fops);
	dev->cdev.owner = THIS_MODULE;
	sema_init(&dev->sem, 1);
	dev->cdev.ops = &fops;
	dev->dev_num = dev_num;
	dev->list_size = list_size;
	dev->quantum_size = quantum_size;
	dev->qset_size = qset_size;
	dev->size = list_size * qset_size * quantum_size;
	dev->list = qset_list_construct(dev->list_size, dev->qset_size, dev->quantum_size);
	if (!dev->list) {
		kfree(dev);
		return NULL;
	}

	err = cdev_add(&dev->cdev, dev->dev_num, 1);
	if (err < 0) {
		printk(KERN_NOTICE "error %d adding scull%d\n", err, index);
		scull_destroy(dev);
		return NULL;
	}

	return dev;
}

void scull_destroy(scull_dev *dev)
{
	cdev_del(&dev->cdev);
	qset_list_destroy(&dev->list);
	kfree(dev);
}
