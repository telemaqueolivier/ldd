/*
 * file_operations.c
 *
 *  Created on: 21 août 2013
 *      Author: telemaque
 */
#include <asm-generic/uaccess.h>
#include "file_operations.h"
#include "scull_dev.h"
#include <linux/ioctl.h>

int scull_open(struct inode *inode, struct file *filep) {
	struct scull_dev *dev; /* device information */

	dev = container_of(inode->i_cdev, struct scull_dev, cdev);
	filep->private_data = dev; /* for other methods */

	/* now trim to 0 the length of the device if open was write-only */
	if ((filep->f_flags & O_ACCMODE) == O_WRONLY) {
//		scull_trim(dev); /* ignore errors */
	}

	return 0;
}

loff_t scull_llseek(struct file *filep, loff_t offset, int whence) {
	struct scull_dev *dev = filep->private_data;
	loff_t newpos;

	switch (whence) {
	case 0: /* SEEK_SET */
		newpos = offset;
		break;

	case 1: /* SEEK_CUR */
		newpos = filep->f_pos + offset;
		break;

	case 2: /* SEEK_END */
		newpos = dev->size + offset;
		break;

	default: /* can't happen */
		return -EINVAL;
	}
	if (newpos < 0)
		return -EINVAL;
	filep->f_pos = newpos;

	return newpos;
}

static void scull_find_quantum(struct scull_dev *dev, loff_t file_pos,
		int *item_pos, int *qset_pos, int *quantum_pos) {
	int quantum_size = dev->quantum_size;
	int item_size = quantum_size * dev->qset_size; /* how many bytes in the listitem */
	int rest;

	*item_pos = (long) file_pos / item_size;
	rest = (long) file_pos % item_size;
	*qset_pos = rest / quantum_size;
	*quantum_pos = rest % quantum_size;
}

ssize_t scull_read(struct file *filep, char __user *buf, size_t count,
		loff_t *file_pos) {
	struct scull_dev *dev = filep->private_data;
	struct quantum_set *qset;
	int quantum_size = dev->quantum_size;
	int item_pos, qset_pos, quantum_pos;
	ssize_t retval = 0;

	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	if (*file_pos >= dev->size)
		goto out;
	if (*file_pos + count > dev->size)
		count = dev->size - *file_pos;
	printk(KERN_INFO "file_pos = %lu\n",(long) *file_pos);
	scull_find_quantum(dev, *file_pos, &item_pos, &qset_pos, &quantum_pos);

	/* follow the list up to the right position (defined elsewhere) */
	qset = qset_list_at(dev->list, item_pos)->qset;

	if (qset == NULL || !quantum_set_has_data(qset)
			|| !quantum_set_at(qset, qset_pos))
		goto out;
	/* don't fill holes */

	/* read only up to the end of this quantum */
	if (count > quantum_size - quantum_pos)
		count = quantum_size - quantum_pos;

//	for (i = 0; i < count; ++i) {
//		*quantum_at(qset, qset_pos, quantum_pos + i) = c++;
//		if (c > 67)
//			c = 65;
//	}

	if (copy_to_user(buf, quantum_at(qset, qset_pos, quantum_pos), count)) {
		retval = -EFAULT;
		goto out;
	}

	*file_pos += count;
	retval = count;

	out: up(&dev->sem);
	return retval;
}

ssize_t scull_write(struct file *filep, const char __user *buf, size_t count,
		loff_t *file_pos) {
	struct scull_dev *dev = filep->private_data;
	struct quantum_set *qset;
	int quantum_size = dev->quantum_size;
	int item_pos, qset_pos, quantum_pos;
	ssize_t retval = -ENOMEM; /* value used in "goto out" statements */

	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;

	scull_find_quantum(dev, *file_pos, &item_pos, &qset_pos, &quantum_pos);

	/* follow the list up to the right position */
	qset = qset_list_at(dev->list, item_pos)->qset;

	if (qset == NULL || !quantum_set_has_data(qset)
			|| !quantum_set_at(qset, qset_pos))
		goto out;
	/* don't fill holes */

	/* write only up to the end of this quantum */
	if (count > quantum_size - qset_pos)
		count = quantum_size - qset_pos;
	if (copy_from_user(quantum_at(qset, qset_pos, quantum_pos), buf, count)) {
		retval = -EFAULT;
		goto out;
	}

	*file_pos += count;
	retval = count;
	/* update the size */
	if (dev->size < *file_pos)
		dev->size = *file_pos;

	out: up(&dev->sem);
	return retval;
}

#define MAGIC_NUM 0xFE
#define IOC_TEST _IO(MAGIC_NUM, 0)
#define IOC_GET_QUANTUM _IOR(MAGIC_NUM, 1, unsigned int)

long scull_ioctl(struct file *filep, unsigned int cmd, unsigned long arg) {
	int ret = 0;
	int ok;

	if (_IOC_DIR(cmd) & _IOC_READ)
		ok = access_ok(VERIFY_WRITE, (void __user *) arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		ok = access_ok(VERIFY_READ, (void __user *) arg, _IOC_SIZE(cmd));
	if (!ok)
		return -EFAULT;

	switch (cmd) {
	case IOC_TEST:
		printk(KERN_INFO "IOCTEST\n");
		break;
	case IOC_GET_QUANTUM:
		ret = __put_user( ((scull_dev *) filep->private_data)->quantum_size,
				(unsigned int *) arg);
		break;
	default:
		printk(KERN_INFO "ioctl invalid argument\n");
		ret = -ENOTTY;
		break;
	}

	return ret;
}

int scull_release(struct inode *inode, struct file *filep) {
	struct scull_dev *dev = filep->private_data; /* device information */

	//qset_list_free(dev->list);

	return 0;
}
