/*
 * quantum_set.h
 *
 *  Created on: 17 août 2013
 *      Author: telemaque
 */

#ifndef QUANTUM_SET_H_
#define QUANTUM_SET_H_

#define foreach(array, size, func) 	\
	unsigned int i;					\
	for (i = 0; i < size; ++i)		\
		func(array[i])

typedef struct quantum_set {
	unsigned int size;
	unsigned int quantum_size;
	void **data;
} quantum_set;

quantum_set* quantum_set_alloc(unsigned int size, unsigned int quantum_size);
void quantum_set_free(quantum_set *qset);
int quantum_set_has_data(quantum_set *qset);
void * quantum_set_at(quantum_set *qset, unsigned int pos);
char * quantum_at(	quantum_set *qset,
					unsigned int qset_pos,
					unsigned int quantum_pos);
#endif /* QUANTUM_SET_H_ */
