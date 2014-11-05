/*
 * sleepy_dev.h
 *
 *  Created on: 30 oct. 2014
 *      Author: telemaque
 */

#ifndef sleepy_DEV_H_
#define sleepy_DEV_H_

#include <linux/types.h>
#include <linux/wait.h>

typedef struct sleepy_dev sleepy_dev_t;

sleepy_dev_t * sleepy_dev_construct(unsigned int count, const char * name);
void sleepy_dev_destroy(sleepy_dev_t * dev);
wait_queue_head_t * wait_queue(sleepy_dev_t * dev);
int flag(sleepy_dev_t * dev);
void set_flag(sleepy_dev_t * dev, int value);
#endif /* sleepy_DEV_H_ */
