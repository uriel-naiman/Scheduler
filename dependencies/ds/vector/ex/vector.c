/******************************************************************
* File Name: ilrd_vector.c										  *
* Name: Uriel Naiman											  *
* Date: 8.6.2020												  *
* Reviewed by: Guy Chen					 						  *
* Topic: Vector C file											  *
*******************************************************************/
#include <stddef.h>			/*size_t*/
#include <stdlib.h>			/*malloc, realloc, free*/
#include <stdio.h>			/*printf*/
#include <assert.h>			/*assert*/

#include "vector.h"

#define FAIL 1
#define SUCCESS 0
#define TWO 2
#define FOUR 4

struct vector
{
    void **data_s;
    size_t size;
    size_t initial_capacity;
    size_t current_capacity;
};

/*---------------------------------------------------------------*/

vector_t *VectorCreate(size_t capacity)
{
	vector_t *vector = (vector_t*) malloc(sizeof(vector_t));

	if (NULL == vector)
	{
		return (NULL);
	}
	
	vector->data_s = malloc(capacity * sizeof(void*));
	vector->size = 1;
	vector->initial_capacity = capacity;
	vector->current_capacity = capacity;
	
	if (NULL == vector->data_s)
	{
		free(vector);
		vector = NULL;
		return (NULL);
	}
	
	return (vector);
}

/*---------------------------------------------------------------*/

void VectorDestroy(vector_t *vector)
{
	assert(vector);
	free(vector->data_s);
	vector->data_s = NULL;
	free(vector);
	vector = NULL;
}

/*----------------------------------------------------------------*/

void *VectorGetElement(const vector_t *vector, size_t index)
{
	if (vector->size <= index)
	{
		return (NULL);
	}
	return (vector->data_s[index]);
}
/*---------------------------------------------------------------------------*/
void VectorSetElement(vector_t *vector, size_t index, void *element)
{
	if (vector->size <= index)
	{
		return;
	}
	vector->data_s[index] = element;
}

/*----------------------------------------------------------------*/

int VectorPushBack(vector_t *vector, void *element)
{
	void *temp = NULL;
	
	if (((vector->size) == (vector->current_capacity)))
	{
		temp = realloc(vector->data_s, (vector->size) * TWO);
		
		if (NULL == temp)
		{
			return (FAIL);
		}
		vector->data_s = temp;
		vector->current_capacity *= TWO;
	}
	
	vector->data_s[vector->size] = element;
	++vector->size;
	
	return (SUCCESS);
}

/*----------------------------------------------------------------*/

void VectorPopBack(vector_t *vector)
{
	if (0 < vector->size)
	{
		if ((vector->size) <= vector->current_capacity / FOUR) 
		{
			if (vector->current_capacity / TWO > vector->
										initial_capacity)
			{
				vector->data_s = realloc(vector->data_s, (vector->
				current_capacity / 2) * sizeof(void*));
				vector->current_capacity /= TWO;
			}
			else
			{
				vector->data_s = realloc(vector->data_s, vector->
						initial_capacity * sizeof(void*));
				vector->current_capacity = vector->initial_capacity;
			}
		}
		--vector->size;
	}
}

/*----------------------------------------------------------------*/

size_t VectorSize(const vector_t *vector)
{
	return (vector->size + 1);
}

/*----------------------------------------------------------------*/

size_t VectorCapacity(const vector_t *vector)
{
	return (vector->current_capacity);
}

/*---------------------------------------------------------------------------*/

int VectorReserve(vector_t *vector, size_t new_capacity)
{
	void *temp = NULL;
	if (new_capacity > vector->current_capacity)
	{
		temp = realloc(vector->data_s, new_capacity * 
											sizeof(void*));
		if 	(NULL == temp)
		{
			return (FAIL);
		}
		vector->data_s = temp;
		vector->current_capacity = new_capacity;
	}
		
	return (SUCCESS);
}

/*----------------------------------------------------------------*/

void VectorShrinkToFit(vector_t *vector)
{
	if (vector->size > vector->initial_capacity)
	{
		vector->data_s = realloc(vector->data_s, vector->size * 
											sizeof(void*));
		vector->current_capacity = vector->size;
	}
	else
	{
		vector->data_s = realloc(vector->data_s, vector->
						initial_capacity * sizeof(void*));
		vector->current_capacity = vector->initial_capacity;
	}
}

/*----------------------------------------------------------------*/

void **VectorGetArr(const vector_t *vector);
{
	return (vector->data_s);
}

