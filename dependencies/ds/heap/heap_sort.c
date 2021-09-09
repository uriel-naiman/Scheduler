/**********************************************************************
* File Name: heap_sort.c         					    				  *
* Name: Uriel Naiman								    			  *
* Create Date: 11.8.2020	    									  *
* Reviewed by: Shai Hasid           							 	  *
* Topic: Heap C file            									  *
**********************************************************************/
#include <stddef.h> 	/* size_t */
#include <assert.h>     /* assert */
#include <stdio.h>

#include "heap_sort.h"
#include "heapify.h"
#include "vector.h"

static void MaxHeapCreate(void *base, size_t nmemb, size_t size,
                  heapify_cmp_func_t cmp,
                  void *arg);


void HeapSort(void *base, size_t nmemb, size_t size,
                  heapify_cmp_func_t cmp,
                  void *arg)
{
    char *ch_base = base;
    MaxHeapCreate(base, nmemb, size, cmp, arg);

    while (nmemb > 1)
    {
        Swap (base, ch_base + (nmemb -1) * size, size);
        --nmemb;
        HeapifyDown(base, nmemb , 0, size, cmp, arg);
        
    }
}



static void MaxHeapCreate(void *base, size_t nmemb, size_t size,
                  heapify_cmp_func_t cmp,
                  void *arg)
{
    char *ch_base = base;
    size_t i = nmemb / 2 - 1;

    while (i < nmemb)
    {
        HeapifyDown(base, nmemb, i, size, cmp, arg);
        
        --i;
    }
    
    printf("max heap:\n");
    for (i = 0; i < 20; ++i)
    {
        printf("%d, ", *(int*)(ch_base + i * sizeof(int)));
    }
    printf("\n");

}
