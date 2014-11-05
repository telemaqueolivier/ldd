/*
 * file_operations.c
 *
 *  Created on: 2 nov. 2014
 *      Author: telemaque
 */
#include <linux/sched.h>
#include <linux/wait.h>
#include "sleepy_dev.h"
#include "file_operations.h"

extern sleepy_dev_t * dev;

ssize_t sleepy_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
	printk(KERN_INFO "process %i (%s) is going to sleep!\n", current->pid, current->comm);
	wait_event_interruptible(*wait_queue(dev), flag(dev) != 0);
	set_flag(dev, 0);
	printk(KERN_INFO "process %i (%s) has been woken up!\n", current->pid, current->comm);

	return 0;
}

ssize_t sleepy_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
	printk(KERN_INFO "process %i (%s) awakes the readers\n", current->pid, current->comm);
	set_flag(dev, 1);
	wake_up_interruptible(wait_queue(dev));

	return count;
}
