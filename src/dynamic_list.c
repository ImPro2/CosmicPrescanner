#include "dynamic_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

dynamic_list_t* dynamic_list_init(int capacity, size_t element_size)
{
	dynamic_list_t* l = malloc(sizeof(dynamic_list_t));

	memset(l, 0, sizeof(dynamic_list_t));
	l->count        = 0;
	l->capacity     = capacity;
	l->element_size = element_size;
	dynamic_list_alloc(l);

	return l;
}

void dynamic_list_free(dynamic_list_t* l)
{
	for (int i = 0; i < l->capacity; i++)
	{
		free(l->elements[i]);
	}

	free(l->elements);
	free(l);
}

void dynamic_list_add(dynamic_list_t* l, void* element)
{
	if (l->count == l->capacity)
	{
		l->capacity *= 2;
		l->elements = realloc(l->elements, l->capacity * l->element_size);

		for (int i = l->count; i < l->capacity; i++)
		{
			l->elements[i] = malloc(l->element_size);
			memset(l->elements[i], 0, sizeof(l->element_size));
		}
	}

	l->elements[l->count++] = element;

	/*
	void* dst = l->elements[l->count++];
	void* src = element;
	//memcpy(dst, src, l->element_size);
	dst = src;
	*/
}

void dynamic_list_alloc(dynamic_list_t* l)
{
	l->elements = calloc(l->capacity, sizeof(void*));
	for (int i = 0; i < l->capacity; i++)
	{
		l->elements[i] = malloc(sizeof(l->element_size));
		memset(l->elements[i], 0, sizeof(l->element_size));
	}
}

bool dynamic_list_contains(dynamic_list_t* l, void* element)
{
	for (int i = 0; i < l->count; i++)
	{
		if (l->elements[i] == element)
			return true;
	}

	return false;
}

