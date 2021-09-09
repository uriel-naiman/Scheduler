/**********************************************************************
* File Name: heapify.c         					    				  *
* Name: Uriel Naiman								    			  *
* Create Date: 11.8.2020	    									  *
* Reviewed by: Shai Hasid           							 	  *
* Topic: Heapify C file            									  *
**********************************************************************/
#include <stdlib.h>     /* malloc, free*/
#include <string.h>     /* memcpy */
#include <assert.h>

#include "heapify.h"


void Swap(char *data1, char *data2, size_t n_bytes);

/*void HeapifyUp(void *base, size_t nmemb, size_t index, size_t elem_size,     
               heapify_cmp_func_t cmp, void *param)
{
    size_t parent = (index - 1) / 2;
    (void)param;
    (void)nmemb;

    while(index > 0)
    {
        int cmpr = cmp(((char*)base + (index * elem_size)), 
        ((char*)base + (parent * elem_size)), NULL);

        if(cmpr < 0) 
        {
            Swap(((char*)base + (index * elem_size)), 
                ((char*)base + (parent * elem_size)), elem_size);
            index = parent;
            parent = (index - 1) / 2;
        }
        else
        {
            return;
        }  
    }
}*/

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

/*void HeapifyDown(void *base, size_t nmemb, size_t index, size_t elem_size,     
                 heapify_cmp_func_t cmp, void *param)
{
    size_t left = index * 2 + 1;
    size_t right = index * 2 + 2;
    char *ch_base = base;

    while (index < (nmemb / 2))
    {
        if (cmp((ch_base + left * elem_size), 
            (ch_base + right * elem_size), param) < 0)
        {
            Swap((ch_base + index * elem_size), 
            (ch_base + left * elem_size), elem_size);
            index = left;
            left = index * 2 + 1;
            right = index * 2 + 2;
        }
        else if (cmp((ch_base + left * elem_size), 
            (ch_base + right * elem_size), param) > 0)
        {
            Swap((ch_base + index * elem_size), 
            (ch_base + right * elem_size), elem_size);
            index = right;
            left = index * 2 + 1;
            right = index * 2 + 2;
        }
        else 
        {
            return;
        }

    }
}*/

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