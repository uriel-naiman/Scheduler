/*****************************************
****** File Name - vector.c         ******
****** Description - vector         ******
****** Developer - Reut             ******
****** Reviewer - sharazad          ******
****** Review Date - 08/6/20        ******
*****************************************/

#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h> 

#include "vector.h"

#define SHRINK_LIMIT 2 
#define SUCCESS 0
#define FAILURE 1

struct vector
{
    size_t size; 
    size_t current_capacity;  
    size_t initail_capacity; 
    void **data; 
};
 

vector_t *VectorCreate(size_t capacity)
{
	vector_t *pt = NULL;

	pt = (vector_t *) malloc (sizeof(vector_t)); 
	if (NULL == pt)	
	{
		return (NULL);
	}

	pt->data = malloc (capacity * sizeof(void *));
	if (NULL == pt->data)
	{
		free(pt);
		pt = NULL;
		return(NULL);
	}
	pt->size = 0;
	pt->initail_capacity = capacity;
	pt->current_capacity = pt->initail_capacity;
	
	return (pt);
}

void VectorDestroy(vector_t *vector)
{
	assert(vector);

	free(vector->data);
	vector->data = NULL;

	free(vector);
	vector = NULL;
}

int VectorPushBack(vector_t *vector, void *element)
{
	vector_t *tmp = vector;
	if (VectorSize(vector) == vector->current_capacity)
	{
		tmp->data = realloc (vector->data, sizeof(void *) * 
						   (vector->current_capacity * 2));
		if (NULL == tmp->data)	
		{
			return (FAILURE);
		}
		vector->data = tmp->data;
		vector->current_capacity *= 2;
	}
	vector->data[vector->size++] = element;
	return (SUCCESS);	
}


void VectorPopBack(vector_t *vector)
{
	
	if (VectorSize(vector))
	{
		--(vector->size);
	}
	
	if (VectorSize(vector) <=  vector->current_capacity / SHRINK_LIMIT &&
		vector->current_capacity / 2 >= vector->initail_capacity)
	{
		vector->current_capacity /= 2;
		vector->data = realloc (vector->data, sizeof(void *) * 
					   (vector->current_capacity / 2));
	}
}


void *VectorGetElement(const vector_t *vector, size_t index)
{
	assert(index < vector->size);
	return (*(vector->data + index));
}


void VectorSetElement(vector_t *vector, size_t index, void *element)
{
	assert(index < vector->size);
	*(vector->data + index) = element;
}


size_t VectorSize(const vector_t *vector)
{
	return (vector->size);
}


size_t VectorCapacity(const vector_t *vector)
{
	return (vector->current_capacity);
}



int VectorReserve(vector_t *vector, size_t new_capacity)
{
	vector_t *tmp = vector;
	
	if (new_capacity > vector->current_capacity)
	{
		tmp->data = realloc (vector->data, sizeof(void *) * new_capacity);
		if (NULL == tmp->data)	
		{
			return (FAILURE);
		}
		vector->data = tmp->data;
		vector->current_capacity = new_capacity;
	}
	return(SUCCESS);
}

void VectorShrinkToFit(vector_t *vector)
{

	if (VectorSize(vector) <= vector->initail_capacity)
	{
		vector->current_capacity = vector->initail_capacity;	
	}
	else
	{
		vector->current_capacity = vector->size;
	}
	vector->data = realloc (vector->data, 
				   sizeof(void *) * vector->current_capacity);
}

void **VectorGetArr(const vector_t *vector)
{
    return (vector->data);
}
