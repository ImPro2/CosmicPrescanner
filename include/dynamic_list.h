#ifndef DYNAMIC_LIST_H
#define DYNAMIC_LIST_H
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
	void** elements;
	int    count;
	int    capacity;
	size_t element_size;
} dynamic_list_t;

dynamic_list_t* dynamic_list_init(int capacity, size_t element_size);
void            dynamic_list_free(dynamic_list_t* l);

void dynamic_list_add(dynamic_list_t* l, void* element);
bool dynamic_list_contains(dynamic_list_t* l, void* element);

void dynamic_list_alloc(dynamic_list_t* l);

#endif
