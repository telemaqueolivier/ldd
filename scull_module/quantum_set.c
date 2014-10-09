/*
 * quantum_set.c
 *
 *  Created on: 17 août 2013
 *      Author: telemaque
 */
#include <linux/slab.h>
#include "quantum_set.h"

quantum_set* quantum_set_alloc(unsigned int size, unsigned int quantum_size)
{
	quantum_set *qset = kmalloc(sizeof(quantum_set), GFP_KERNEL);
	unsigned int i, j;

	if (!qset) {
		printk(KERN_WARNING "can't allocate quantum set!");
		return NULL;
	}

	qset->size = size;
	qset->data = kmalloc(qset->size * sizeof (void *), GFP_KERNEL);
	if (!qset->data) {
		kfree(qset);
		printk(KERN_WARNING "can't allocate quantum set data!");
		return NULL;
	}

	qset->quantum_size = quantum_size;
	for (i = 0; i < qset->size; ++i) {
		qset->data[i] = kmalloc(qset->quantum_size, GFP_KERNEL);
		if (!qset->data[i]) {//error code
			for(j = 0; j < i; ++j)
				kfree(qset->data[j]);
			kfree(qset->data);
			kfree(qset);
			printk(KERN_WARNING "can't allocate quantum data!");
			return NULL;
		}
	}

	return qset;
}

int quantum_set_has_data(quantum_set *qset)
{
	return qset->data ? 1 : 0;
}

void * quantum_set_at(quantum_set *qset, unsigned int pos)
{
	return qset->data[pos];
}

void quantum_set_free(quantum_set *qset)
{
	unsigned int i;

	for (i = 0; i < qset->size; ++i)
		kfree(qset->data[i]);

	kfree(qset->data);
	kfree(qset);
}

char * quantum_at(	quantum_set *qset,
					unsigned int qset_pos,
					unsigned int quantum_pos)
{
	return ((char *) qset->data[qset_pos]) + quantum_pos;
}
