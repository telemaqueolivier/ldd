/*
 * scull_dev.h
 *
 *  Created on: 21 août 2013
 *      Author: telemaque
 */

#ifndef SCULL_DEV_H_
#define SCULL_DEV_H_

#include <linux/cdev.h>
#include <linux/semaphore.h>
#include "qset_list.h"

typedef struct scull_dev {
	dev_t dev_num;
	struct cdev cdev;
	struct semaphore sem;
	qset_list *list;
	unsigned int size;
	unsigned int qset_size;
	unsigned int quantum_size;
	unsigned int list_size;
} scull_dev;

scull_dev * scull_construct(int index,
							dev_t dev_num,
							unsigned int list_size,
							unsigned int qset_size,
							unsigned int quantum_size);

void scull_destroy(scull_dev *dev);
#endif /* SCULL_DEV_H_ */
