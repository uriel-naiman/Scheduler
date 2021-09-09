/******************************************************************
* File Name: dlist.c											  *
* Name: Uriel Naiman											  *
* Topic: Dlist C file										  	  *
*******************************************************************/
#include <stdio.h>		/* NULL */
#include <stdlib.h>		/* malloc, free*/
#include <assert.h> 	/* assert*/

#include "dlist.h"		/* dlist header file*/

struct dlist_node
{
    void *data;
    dlist_iter_t next;
    dlist_iter_t prev;	
};

struct dlist
{
	dlist_iter_t head;
    dlist_iter_t tail;
};

static dlist_iter_t NodeCreate(dlist_iter_t iter)
{
	return ((dlist_iter_t) malloc(sizeof(struct dlist_node)));
	
	if (NULL == iter)
	{
		return (NULL);
	}
	
	return (iter);
}

/*----------------------------------------------------------------*/
dlist_t *DListCreate(void)
{
	dlist_iter_t head_dummy = NULL;
	dlist_iter_t tail_dummy = NULL;
	dlist_t *dlist = (dlist_t*) malloc(sizeof(dlist_t));
	
	if (NULL == dlist)
	{
		return (NULL);
	}
	
	head_dummy = NodeCreate(head_dummy);
	
	if (NULL == head_dummy)
	{
		free(dlist);
		dlist = NULL;
		return (NULL);
	}
	
	tail_dummy = NodeCreate(tail_dummy);
	
	if (NULL == tail_dummy)
	{
		free(dlist);
		free(head_dummy);
		dlist = NULL;
		head_dummy = NULL;
		return (NULL);
	}
	
	head_dummy->data = (void*)0xBADC0FFEE0DDF00D;
	head_dummy->next = tail_dummy;
	head_dummy->prev = NULL;
	
	tail_dummy->data = (void*)0xBADC0FFEE0DDF00D;
	tail_dummy->next = NULL;
	tail_dummy->prev = head_dummy;
	
	dlist->head = head_dummy;
	dlist->tail = tail_dummy;

	return (dlist);
}

/*----------------------------------------------------------------*/

void DListDestroy(dlist_t *dlist)
{
	dlist_iter_t curr = DListPrev(DListBegin(dlist));
	dlist_iter_t next = DListNext(curr);
	assert(dlist);
	
	while (NULL != next)
	{
		free(curr);
		curr = next;
		next = DListNext(next);
	}
	free(curr);
	curr = NULL;
	free(dlist);
	dlist = NULL;
}

/*----------------------------------------------------------------*/

dlist_iter_t DListBegin(const dlist_t *dlist)
{
	assert(dlist);
	return (dlist->head->next);
}

/*---------------------------------------------------------------*/

dlist_iter_t DListEnd(const dlist_t *dlist)
{
	assert(dlist);
	return (DListNext(dlist->tail->prev));
}

/*---------------------------------------------------------------*/

dlist_iter_t DListNext(dlist_iter_t iter)
{
	assert(iter);
	return (iter->next);
}

/*---------------------------------------------------------------*/

dlist_iter_t DListPrev(dlist_iter_t iter)
{
	assert(iter);
	return (iter->prev);
}

/*---------------------------------------------------------------*/

void *DListGetData(dlist_iter_t iter)
{
	assert(iter);
	return (iter->data);
}

/*---------------------------------------------------------------*/

void DListSetData(dlist_iter_t iter, void *data)
{
	assert(iter);
	iter->data = data;
}

/*---------------------------------------------------------------*/

int DListIterIsEqual(dlist_iter_t iter1, dlist_iter_t iter2)
{
	assert(iter1);
	assert(iter2);
	
	return (iter1 == iter2);	
}

/*---------------------------------------------------------------*/

int DListIsEmpty(const dlist_t *dlist)
{
	assert(dlist);
	return (DListBegin(dlist) == DListEnd(dlist));
}

/*---------------------------------------------------------------*/

dlist_iter_t DListInsert(dlist_t *dlist, dlist_iter_t where, void 
*data)
{
	dlist_iter_t new_node = NULL;
	
	assert(dlist);
	assert(where);
	
	new_node = NodeCreate(new_node);
	
	if (NULL == new_node)
	{
		return (dlist->tail);
	}
	
	new_node->data = data;
	new_node->next = where;
	new_node->prev = DListPrev(where);
	
	DListPrev(new_node)->next = new_node;
	DListNext(new_node)->prev = new_node;
	
	return (new_node);
}

/*---------------------------------------------------------------*/

dlist_iter_t DListRemove(dlist_iter_t iter)
{
	dlist_iter_t temp = DListNext(iter);
	
	assert(iter);
	
	DListPrev(iter)->next = DListNext(iter);
	DListNext(iter)->prev = DListPrev(iter);
	
	free(iter);
	iter = NULL;
	
	return (temp);
}
	

/*---------------------------------------------------------------*/

dlist_iter_t DListPushFront(dlist_t *dlist, void *data)
{
	assert(dlist);
	return (DListInsert(dlist, DListBegin(dlist), data));
}

/*---------------------------------------------------------------*/

dlist_iter_t DListPushBack(dlist_t *dlist, void *data)
{
	assert(dlist);
	return (DListInsert(dlist, DListEnd(dlist), data));
}

/*---------------------------------------------------------------*/

void *DListPopFront(dlist_t *dlist)
{
	void *temp = NULL;
	
	assert(dlist);
	
	temp = DListGetData(DListBegin(dlist));
	DListRemove(DListBegin(dlist));
	
	return (temp);
}

/*---------------------------------------------------------------*/

void *DListPopBack(dlist_t *dlist)
{
	void *temp = NULL;
	
	assert(dlist);
	
	temp = DListGetData(DListEnd(dlist)->prev);
	DListRemove(DListEnd(dlist)->prev);
	
	return (temp);
}

/*---------------------------------------------------------------*/

size_t DListSize(const dlist_t *dlist)
{
	size_t count = 0;
	dlist_iter_t temp = NULL;
	
	assert(dlist);
	
	temp = DListBegin(dlist);
	
	while (NULL != temp->next)
	{
		++count;
		temp = DListNext(temp);
	}
	
	return (count);
}

/*---------------------------------------------------------------*/

dlist_iter_t DListFind(dlist_iter_t from, 
                       const dlist_iter_t to,
					   dlist_is_match_func_t is_match, 
                       void *param)
{
	while (!(DListIterIsEqual(from, to)) && (!is_match(from->
	data, param)))
	{
			from = DListNext(from);
	}
	
	return (from);
}

/*---------------------------------------------------------------*/

int DListForEach(dlist_iter_t from, 
                 const dlist_iter_t to, 
				 dlist_do_action_func_t do_action, 
                 void *param)
{
	while (!(DListIterIsEqual(from, to)) && (do_action(from->
	data, param)))
	{	
		from = DListNext(from);
	}
	
	return (DListIterIsEqual(from, to));
}
                 
/*---------------------------------------------------------------*/

dlist_iter_t DListSplice(dlist_iter_t from, dlist_iter_t to, 
dlist_iter_t where)
{
	/* temp iterators ends */
	dlist_iter_t to_temp = DListPrev(to);
	dlist_iter_t src_from = DListPrev(from);
	dlist_iter_t where_prev = DListPrev(where);
	
	/* connecting src ends */
	src_from->next = to;
	to->prev = src_from;
	
	/* adding part to dest dlist*/
	where_prev->next = from;
	from->prev = where_prev;
	to_temp->next = where;
	where->prev = to_temp;
	
	return (to_temp);
}
	

