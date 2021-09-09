#ifndef HEAP_SORT_H
#define HEAP_SORT_H

#include <stddef.h>	/* size_t */
#include "heapify.h"

void HeapSort(void *base, size_t nmemb, size_t size,
                  heapify_cmp_func_t cmp,
                  void *arg);         

#endif /* HEAP_SORT_H */ 