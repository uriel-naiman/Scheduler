/**********************************************************************
* File Name: heap.c         					    				  *
* Name: Uriel Naiman								    			  *
* Topic: Heap C file            									  *
**********************************************************************/
#include <stdlib.h>     /* malloc, free */
#include <stddef.h> 	/* size_t */
#include <assert.h>     /* assert */
#include <stdio.h>

#include "heap.h"
#include "heapify.h"
#include "vector.h"

struct heap 
{
    vector_t *vector;
    heap_cmp_func_t cmp;
    void *param;
};

/*---------------------------------------------------------------------------*/

heap_t* HeapCreate(heap_cmp_func_t cmp, void *param)
{
    heap_t *heap = (heap_t*) malloc(sizeof(heap_t));
    if (NULL == heap)
    {
        return (NULL);
    }
    heap->vector = VectorCreate(10);
    if (NULL == heap->vector)
    {
        free(heap);
        heap = NULL;
        return (NULL);
    }

    heap->cmp = cmp;
    heap->param = param;
    
    
    return (heap);
}

/*---------------------------------------------------------------------------*/

void HeapDestroy(heap_t *heap)
{
    VectorDestroy(heap->vector);
    free(heap);
    heap = NULL;
}

/*---------------------------------------------------------------------------*/

/*int HeapPush(heap_t *heap, void *data)
{
    size_t nmemb = 0;
    void * base = VectorGetArr(heap->vector);
    size_t index = VectorSize(heap->vector);

    if (EXIT_FAILURE == VectorPushBack(heap->vector, data))
    {
        return(EXIT_FAILURE);
    }

    HeapifyUp(base, nmemb, index, sizeof(void*), heap->cmp, heap->param);

    return (EXIT_SUCCESS);
}*/

int HeapPush(heap_t *heap, void *data)
{
        if(VectorPushBack(heap->vector, data) == 1)
        {
            return (1); /* failure */
        }

        HeapifyUp(VectorGetArr(heap->vector), VectorSize(heap->vector), VectorSize(heap->vector) - 1, sizeof(void*), heap->cmp, heap->param);
        return (0);  
}

/*---------------------------------------------------------------------------*/

void *HeapPeek(heap_t *heap)
{
    return *(VectorGetArr(heap->vector));
}

/*---------------------------------------------------------------------------*/

/*void *HeapPop(heap_t *heap)
{
    void *base = NULL;
    char *ch_base = NULL;
    size_t last_index = 0;
    void *element = NULL;
    size_t size = sizeof(void*);
    size_t nmemb = 0;

    assert(heap);
    
    if (0 == VectorSize(heap->vector))
    {
        return (NULL);
    }

    base = VectorGetArr(heap->vector);
    ch_base = base;
    last_index = VectorSize(heap->vector) - 1;
    element = VectorGetElement(heap->vector, 0);
    nmemb = VectorSize(heap->vector) - 1;

    Swap(base, ch_base + last_index * size, size);

    VectorPopBack(heap->vector);

    HeapifyDown(base, nmemb, 0, size, heap->cmp, heap->param);

    return (element);
}*/


void *HeapPop(heap_t *heap)
{
    void *return_value = NULL;
    void *last_element = NULL;

    assert(heap);

    return_value = HeapPeek(heap);
    last_element = VectorGetElement(heap->vector, VectorSize(heap->vector) - 1);
    VectorSetElement(heap->vector, 0, last_element);

    VectorPopBack(heap->vector);

    HeapifyDown(VectorGetArr(heap->vector), VectorSize(heap->vector), 0, sizeof(void*), heap->cmp, heap->param);

    return (return_value);
}
/*---------------------------------------------------------------------------*/

void *HeapRemove(heap_t *heap, void *data, heap_is_match_func_t is_match)
{
    size_t i = 0;
    void *result = NULL;

    assert(heap);
    
    while( i < VectorSize(heap->vector) && is_match(VectorGetElement(heap->vector, i), data, NULL))
    {
        ++i;
    }
    
    if (i == VectorSize(heap->vector))
    {
        return (result);
    }

    result = VectorGetElement(heap->vector, i);
    VectorSetElement(heap->vector, i, VectorGetElement(heap->vector, VectorSize(heap->vector) - 1));
    VectorPopBack(heap->vector);

    HeapifyDown(VectorGetArr(heap->vector), VectorSize(heap->vector), i, sizeof(void *), 
                        heap->cmp, heap->param);

    return(result);
}


/*---------------------------------------------------------------------------*/

size_t HeapSize(heap_t *heap)
{
    return (VectorSize(heap->vector));
}

/*---------------------------------------------------------------------------*/

int HeapIsEmpty(heap_t *heap)
{
    return (HeapSize(heap) == 0);
}


void PrintIntVector(heap_t *heap)
{
    size_t i = 0;

    for(i = 0; i < VectorSize(heap->vector); ++i)
    {
        printf("%d, ", *(int *)VectorGetElement(heap->vector, i));
    }

    printf("\n");
}
