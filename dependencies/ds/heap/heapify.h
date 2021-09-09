/* heap util file, contains funtions to support Heap DS.

-------------------------------------------------------------------------------

Util functions include:
-   HeapifyUp - re-arranges the array to have a heap property, going bottom-up
-   HeapifyDown - re-arranges the array to a have heap property, going up-bottom

*/

#ifndef HEAP_UTIL_H
#define HEAP_UTIL_H 

#include <stddef.h>    /* size_t */


/* compare function, returns number less than zero if data1 is lesser than
   data2, nubmer greater than zero if data1 is greater than data2 and zero 
   if equal */
typedef int (*heapify_cmp_func_t)(const void *data1, const void *data2, void *param);


/*---------------------------------------------------------------------------*/

/* HeapifyUp:
*   re-arranges the array to have a heap property, going bottom-up
* 
* Args:
*  - arr - pointer to the array to transform to heap
*  - elem_size - size of elements stored in the array
*	- cmp - pointer compare function implementerd by the user
*	- param - parameter for cmp function
*  - index - index of the node to swap up
* 
*/
void HeapifyUp(void *base, size_t nmemb, size_t index, size_t elem_size,     
               heapify_cmp_func_t cmp, void *param);

/*---------------------------------------------------------------------------*/

/* HeapifyDown:
*   re-arranges the array to have a heap property, going up-bottom
* 
* Args:
*  - base - array to transform to heap
*  - elem_size - size of elements stored in the array
*	- cmp - pointer compare function implementerd by the user
*	- param - parameter for cmp function
*  - index - index of the node to swap down
*
*/
void HeapifyDown(void *base, size_t nmemb, size_t index, size_t elem_size,     
                 heapify_cmp_func_t cmp, void *param);

/*---------------------------------------------------------------------------*/

void Swap(char *data1, char *data2, size_t n_bytes);

#endif  /* ILRD_HEAP_UTIL_H */