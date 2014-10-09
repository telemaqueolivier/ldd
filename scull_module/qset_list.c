/*
 * qset_list.c
 *
 *  Created on: 17 août 2013
 *      Author: telemaque
 */
#include <linux/slab.h>
#include "qset_list.h"

qset_list * qset_list_construct(	unsigned int list_size,
									unsigned int qset_size,
									unsigned int quantum_size)
{
	unsigned int i;
	qset_list * list;
	qset_list_element *head;

	list = (qset_list*) kmalloc(sizeof(qset_list), GFP_KERNEL);
	if (!list)
		return NULL;

	list->qset_size = qset_size;
	list->quantum_size = quantum_size;
	head = qset_list_element_construct(qset_size, quantum_size);
	if (!head) {
		kfree(list);
		return NULL;
	}
	list->head = head;

	for (i = 1; i < list_size; ++i)
		if (qset_list_add(list) != 0) {
			unsigned int j;

			for (j = 0; j < i; ++j)
				qset_list_destroy_at(list, j);

			kfree(head);
			kfree(list);

			return NULL;
		}

	return list;
}

qset_list_element* qset_list_at(qset_list *list, unsigned int index)
{
	unsigned int i;
	qset_list_element *curr = list->head;

	if (index >= qset_list_size(list))
		return NULL;

	for (i = 0; i < index; ++i)
		curr = curr->next;

	return curr;
}

qset_list_element* qset_list_last(qset_list *list)
{
	qset_list_element *curr;

	for (curr = list->head; curr->next != NULL; curr = curr->next);

	return curr;
}

int qset_list_add(qset_list *list)
{
	qset_list_element *last = qset_list_last(list);

	last->next = qset_list_element_construct(	list->qset_size,
												list->quantum_size);
	if (!last->next)
		return -1;

	return 0;
}

void qset_list_destroy(qset_list **list)
{
	int i, size = qset_list_size(*list);

	for (i = size - 1; i >= 0; --i)
		qset_list_destroy_at(*list, i);

	kfree(*list);
	*list = NULL;
}

void qset_list_destroy_at(qset_list *list, unsigned int element_pos)
{
	qset_list_element * to_destroy = qset_list_at(list, element_pos);
	qset_list_element * previous = qset_list_at(list, element_pos - 1);
	qset_list_element * next = qset_list_at(list, element_pos + 1);

	if (next && previous)
		previous->next = next;
	else if (next)
		list->head = next;
	else if (previous)
		previous->next = NULL;
	else
		list->head = NULL;

	qset_list_element_destroy(to_destroy);
}

unsigned int qset_list_size(qset_list *list)
{
	int size = 0;
	qset_list_element * curr = list->head;

	if (!list->head)
		return 0;

	while (curr) {
		++size;
		curr = curr->next;
	}

	return size;
}

qset_list_element * qset_list_element_construct(	unsigned int qset_size,
													unsigned int quantum_size)
{
	qset_list_element *item = (qset_list_element *) kmalloc(sizeof(qset_list_element), GFP_KERNEL);

	if (!item)
		return NULL;

	item->next = NULL;
	item->qset = quantum_set_alloc(qset_size, quantum_size);
	if (!item->qset) {
		kfree(item);
		return NULL;
	}

	return item;
}

void qset_list_element_destroy(qset_list_element * element)
{
	quantum_set_free(element->qset);
	kfree(element);
}

void qset_list_print(qset_list *list)
{
	unsigned int i, size = qset_list_size(list);
	qset_list_element * curr;
	for (i = 0; i < size; ++i) {
		curr = qset_list_at(list, i);
		printk(KERN_INFO "element = %p\n", curr);
		printk(KERN_INFO "element->next = %p\n", curr->next);
		printk(KERN_INFO "      |\n");
		printk(KERN_INFO "     \\|/\n");
	}
}
