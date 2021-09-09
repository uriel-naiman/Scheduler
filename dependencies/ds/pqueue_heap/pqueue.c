/*****************************************************************
* File Name: pqueue.c										  *
* Name: Uriel Naiman										  *
* Topic: pqueue C file										  *
*******************************************************************/
#include <stdio.h>		/* NULL */
#include <assert.h>		/* assert */
#include <stdlib.h>		/* malloc, free */

#include "heap.h"
#include "pqueue.h"


struct pqueue
{
    heap_t *heap;
    pqueue_cmp_func_t cmp;
   	void *param;
};


int IsBefore(const void *data1, const void *data2, void *param)
{
	pqueue_t *pq = (pqueue_t*)param;
	
	return (pq->cmp(data1, data2, pq->param) > 0);
}


/*---------------------------------------------------------------------------*/

pqueue_t *PQCreate(pqueue_cmp_func_t cmp, void *param)
{
	pqueue_t *pq = NULL;
	
	assert(cmp);
	
	pq = (pqueue_t*) malloc(sizeof(pqueue_t));
	
	if (NULL == pq)
	{
		return (NULL);
	}
	
	pq->cmp = cmp;
	pq->heap = HeapCreate(cmp, NULL);
	
	if (NULL == pq->heap)
	{
		free(pq);
		pq = NULL;
		return (NULL);
	}

	(void)(param);
	
	return (pq);
}

/*---------------------------------------------------------------------------*/

void PQDestroy(pqueue_t *pqueue)
{
	assert(pqueue);
	HeapDestroy(pqueue->heap);
	free(pqueue);
	pqueue = NULL;
}

/*---------------------------------------------------------------------------*/

size_t PQSize(const pqueue_t *pqueue)
{
	assert(pqueue);
	return (HeapSize(pqueue->heap));
}

/*---------------------------------------------------------------------------*/

int PQIsEmpty(const pqueue_t *pqueue)
{
	assert(pqueue);
	return (HeapIsEmpty(pqueue->heap));
}

/*---------------------------------------------------------------------------*/

int PQEnqueue(pqueue_t *pqueue, void *data)
{
	assert(pqueue);
	
	return (!HeapPush(pqueue->heap, data));
}

/*---------------------------------------------------------------------------*/

void *PQDequeue(pqueue_t *pqueue)
{
	assert(pqueue);
	return (HeapPop(pqueue->heap));
}

/*---------------------------------------------------------------------------*/

void *PQPeek(const pqueue_t *pqueue)
{
	assert(pqueue);
	return (HeapPeek(pqueue->heap));
}

/*---------------------------------------------------------------------------*/

void PQClear(pqueue_t *pqueue)
{
	assert(pqueue);
	
	while (!PQIsEmpty(pqueue))
	{
		PQDequeue(pqueue);
	}
}

/*---------------------------------------------------------------------------*/

void *PQErase(pqueue_t *pqueue, void *data, pqueue_is_match_func_t is_match, void *param)
{
	assert(pqueue);
	(void)(param);
	return (HeapRemove(pqueue->heap, data, is_match));
		
}
	
	
