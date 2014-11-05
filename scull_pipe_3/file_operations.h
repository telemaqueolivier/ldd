/*
 * file_operations.h
 *
 *  Created on: 2 nov. 2014
 *      Author: telemaque
 */

#ifndef FILE_OPERATIONS_H_
#define FILE_OPERATIONS_H_

#include <linux/fs.h>

ssize_t sleepy_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
ssize_t sleepy_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);

#endif /* FILE_OPERATIONS_H_ */
