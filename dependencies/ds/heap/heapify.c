/**********************************************************************
* File Name: heapify.c         					    				  *
* Name: Uriel Naiman								    			  
* Topic: Heapify C file            									  *
**********************************************************************/
#include <stdlib.h>     /* malloc, free*/
#include <string.h>     /* memcpy */
#include <assert.h>

#include "heapify.h"


void Swap(char *data1, char *data2, size_t n_bytes);

void HeapifyUp(void *base, size_t nmemb, size_t index, size_t elem_size,     
               heapify_cmp_func_t cmp, void *param)
{
    size_t i = index;
    (void)param;
    (void)nmemb;

    while(i > 0)
    {
        if(cmp((char*)base + i * elem_size, (char*)base + (((i-1)/2) * elem_size), NULL) < 1) /* min heap sort */
        {
            Swap((char*)base + i * elem_size, (char*)base + (((i-1)/2) * elem_size), elem_size);
            i = (i - 1)/2;
        }
        else
        {
            return;
        }  
    }
}

/*---------------------------------------------------------------------------*/

void HeapifyDown(void *base, size_t nmemb, size_t index, size_t elem_size,     
                 heapify_cmp_func_t cmp, void *param)
{
    size_t i = index;
    int swap_flag = 1;

    while(swap_flag)
    {   /* min heap sort */
        swap_flag = 0;
        for(i = 0; i <= (nmemb / 2); ++i)
        {
            if(cmp((char*)base + i * elem_size, (char*)base + (2 * i + 1) * elem_size, param) > 0)
            {
                Swap((char*)base + i * elem_size, (char*)base + (2 * i + 1) * elem_size, elem_size);
                swap_flag = 1;
            }
            if(2 * i + 2 < nmemb)
            {
                if(cmp((char*)base + i * elem_size, (char*)base + (2 * i + 2) * elem_size, param) > 0)
                {
                    Swap((char*)base + i * elem_size, (char*)base + (2 * i + 2) * elem_size, elem_size);
                    swap_flag = 1;
                }
            }
        }
    }
}


void Swap(char *data1, char *data2, size_t n_bytes)
{
    char *temp = malloc(n_bytes);

    memcpy(temp, data1, n_bytes);
    memcpy(data1, data2, n_bytes);
    memcpy(data2, temp, n_bytes);

    free(temp);
    temp = NULL;
}