/*
 * file_operations.h
 *
 *  Created on: 21 août 2013
 *      Author: telemaque
 */

#ifndef FILE_OPERATIONS_H_
#define FILE_OPERATIONS_H_

#include <linux/module.h>
#include <linux/fcntl.h>
#include <linux/fs.h>
#include <linux/types.h>

loff_t scull_llseek(struct file *filepp, loff_t offset, int e);
ssize_t scull_read(struct file *filepp, char __user *buf, size_t size, loff_t *offset);
ssize_t scull_write(struct file *filepp, const char __user *buf, size_t size, loff_t *offset);
long scull_ioctl(struct file *filepp, unsigned int e, unsigned long f);
int scull_open(struct inode *inode, struct file *filepp);
int scull_release(struct inode *inode, struct file *filepp);

#endif /* FILE_OPERATIONS_H_ */
