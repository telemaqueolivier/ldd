/*
 * qset_list.h
 *
 *  Created on: 17 août 2013
 *      Author: telemaque
 */

#ifndef QSET_LIST_H_
#define QSET_LIST_H_

#include "quantum_set.h"

#define QUANTUM_SIZE 4000
#define QSET_SIZE 1000
#define LIST_SIZE 20

typedef struct qset_list_element {
	struct qset_list_element *next;
	quantum_set *qset;
} qset_list_element;

typedef struct qset_list {
	qset_list_element *head;
	unsigned int qset_size;
	unsigned int quantum_size;
} qset_list;

qset_list * qset_list_construct(	unsigned int list_size,
									unsigned int qset_size,
									unsigned int quantum_size);
qset_list_element* qset_list_at(qset_list *list, unsigned int index);
qset_list_element* qset_list_last(qset_list *list);
int qset_list_add(qset_list *list);
void qset_list_destroy(qset_list **list);
void qset_list_destroy_at(qset_list *list, unsigned int element_pos);
unsigned int qset_list_size(qset_list *list);
qset_list_element * qset_list_element_construct(unsigned int qset_size,
												unsigned int quantum_size);
void qset_list_element_destroy(qset_list_element * element);
void qset_list_print(qset_list *list);
#endif /* QSET_LIST_H_ */
