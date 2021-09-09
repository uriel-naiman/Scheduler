/******************************************************************
* File Name: sort_list.c										  *
* Name: Uriel Naiman											  *
* Topic: sorted list C file										  *
*******************************************************************/
#include <stdio.h>		/* NULL*/
#include <assert.h>		/* assert*/
#include <stdlib.h>		/* malloc, free*/

#include "sorted_list.h"		/*sorted_list header file*/

struct sorted_list
{
	dlist_t *dlist;
	sorted_list_is_before_func_t is_before; 
	void *param;
};

/*----------------------------------------------------------------*/

sorted_list_t *SortedListCreate(sorted_list_is_before_func_t 
	is_before, void *param)
{
	sorted_list_t *sorted_list = NULL;
	
	assert(is_before);
	
	sorted_list = (sorted_list_t*) malloc(sizeof(sorted_list_t));
	
	if (NULL == sorted_list)
	{
		return (NULL);
	}
	
	sorted_list->dlist = DListCreate();
	sorted_list->is_before = is_before;
	sorted_list->param = param;
	
	if (NULL == sorted_list->dlist)
	{
		free (sorted_list);
		
		sorted_list = NULL;
		
		return (NULL);
	}
	
	return (sorted_list);
}

/*----------------------------------------------------------------*/

void SortedListDestroy(sorted_list_t *sorted_list)
{
	DListDestroy(sorted_list->dlist);
	
	free (sorted_list);
	
	sorted_list = NULL;
}

/*----------------------------------------------------------------*/

sorted_list_iter_t SortedListBegin(const sorted_list_t *sorted_list)
{
	sorted_list_iter_t iter = {0};
	
	assert(sorted_list);
	
	iter.internal_iter = DListBegin(sorted_list->dlist);
	
	return (iter);
}

/*----------------------------------------------------------------*/

sorted_list_iter_t SortedListEnd(const sorted_list_t *sorted_list)
{
	sorted_list_iter_t iter = {0};
	
	assert(sorted_list);
	
	iter.internal_iter = DListEnd(sorted_list->dlist);
	
	return (iter);
}

/*----------------------------------------------------------------*/


sorted_list_iter_t SortedListNext(sorted_list_iter_t iter)
{
	assert(iter.internal_iter);
	
	iter.internal_iter = DListNext(iter.internal_iter);
	
	return (iter);
}

/*----------------------------------------------------------------*/

sorted_list_iter_t SortedListPrev(sorted_list_iter_t iter)
{
	assert(iter.internal_iter);
	
	iter.internal_iter = DListPrev(iter.internal_iter);
	
	return (iter);
}


/*----------------------------------------------------------------*/

void *SortedListGetData(sorted_list_iter_t iter)
{
	assert (iter.internal_iter);
	
	return (DListGetData(iter.internal_iter));
}

/*----------------------------------------------------------------*/

int SortedListIterIsEqual(sorted_list_iter_t iter1,
						  sorted_list_iter_t iter2)
{
	assert(iter1.internal_iter);
	assert(iter2.internal_iter);
	
	return (DListIterIsEqual(iter1.internal_iter, iter2.internal_iter));
}
	
/*----------------------------------------------------------------*/

int SortedListIsEmpty(const sorted_list_t *sorted_list)
{
	assert(sorted_list);
	
	return (DListIsEmpty(sorted_list->dlist));
}

/*----------------------------------------------------------------*/

sorted_list_iter_t SortedListInsert(sorted_list_t *sorted_list, 
	void *data)
{
	sorted_list_iter_t where = SortedListBegin(sorted_list);
			
	assert(sorted_list);
		
	while (!SortedListIterIsEqual(where, SortedListEnd(sorted_list)) 
	&& sorted_list->is_before(SortedListGetData(where),data, 
	sorted_list->param))
	{
			where = SortedListNext(where);
	}
		
	where.internal_iter = DListInsert(sorted_list->dlist, 
		where.internal_iter, data);

	return (where);
}
	
/*----------------------------------------------------------------*/

sorted_list_iter_t SortedListRemove(sorted_list_iter_t iter)
{
	iter.internal_iter = DListRemove(iter.internal_iter);
	
	return (iter);
}

/*----------------------------------------------------------------*/

void *SortedListPopFront(sorted_list_t *sorted_list)
{
	assert(sorted_list);
	
	return (DListPopFront(sorted_list->dlist));
}

/*----------------------------------------------------------------*/

void *SortedListPopBack(sorted_list_t *sorted_list)
{
	assert(sorted_list);
	
	return (DListPopBack(sorted_list->dlist));
}

/*----------------------------------------------------------------*/

size_t SortedListSize(const sorted_list_t *sorted_list)
{
	assert(sorted_list);
	
	return (DListSize(sorted_list->dlist));
}

/*----------------------------------------------------------------*/

sorted_list_iter_t SortedListFind(sorted_list_t *sorted_list,
								  sorted_list_iter_t from,
								  sorted_list_iter_t to,
								  void *to_find)
{
	/* run until find an element greater than to_find */
	while (!SortedListIterIsEqual(from, to) && sorted_list->
	is_before(SortedListGetData(from),to_find, sorted_list->param))
	{
		from = SortedListNext(from);
	}
	
	/* back-up iterator */
	from = SortedListPrev(from);
	
	/* if the iterator == the head dummy, then no find*/
	if  (SortedListIterIsEqual(SortedListNext(from),
								SortedListBegin(sorted_list)))
	{
		return (to);
	}
	
	/* check if 'from' equals to find, by swaping passed variables 
		to 'is_before' */
	if (sorted_list->is_before(to_find, SortedListGetData(from), 
	sorted_list->param))
	{
		return (to);
	}
	
	return (from);
}	
	
/*----------------------------------------------------------------*/

sorted_list_iter_t SortedListFindIf(sorted_list_iter_t from, 
							sorted_list_iter_t to,
							sorted_list_is_match_func_t is_match, 
							void *param)
{
	assert(from.internal_iter);
	assert(to.internal_iter);

	from.internal_iter = DListFind(from.internal_iter,
	to.internal_iter, is_match, param);
	
	return (from);
}

/*----------------------------------------------------------------*/

int SortedListForEach(sorted_list_iter_t from, 
					  sorted_list_iter_t to, 
					  sorted_list_do_action_func_t do_action, 
					  void *param)
{
	assert(from.internal_iter);
	assert(to.internal_iter);

	return (DListForEach(from.internal_iter, to.internal_iter,
					do_action, param));
}
					  

/*----------------------------------------------------------------*/

void SortedListMerge(sorted_list_t *dest, sorted_list_t *src)
{
    sorted_list_iter_t from = {0};
    sorted_list_iter_t to = {0};
    sorted_list_iter_t iter = {0};

    assert(dest);
    assert(src);

    from = SortedListBegin(src);
    iter = SortedListBegin(dest);

    while (!SortedListIterIsEqual(iter, SortedListEnd(dest)) &&
    		!SortedListIterIsEqual(from, SortedListEnd(src)))
    {
        while(!SortedListIterIsEqual(iter, SortedListEnd(dest)) && 
        dest->is_before(SortedListGetData(iter), SortedListGetData(from), dest->param))
        {
            iter = SortedListNext(iter);
        }

        to = SortedListNext(from);

        while (!SortedListIterIsEqual(to, SortedListEnd(src)))
        { 
        	if (dest->is_before(SortedListGetData(iter), 
        						SortedListGetData(to), dest->param))
        	{
            	to = SortedListNext(to);
        	}
        	else
        	{
        		break;
        	}
        }

        DListSplice(from.internal_iter, to.internal_iter, 
        								iter.internal_iter);
        from = SortedListBegin(src);
    }
    
    /* if src list didn't reach end yet and dest does, splices the rest of 
	   src to dest. */
	if (!SortedListIterIsEqual(to, SortedListEnd(src)) &&
		SortedListIterIsEqual(iter, SortedListEnd(dest)))
	{
		from = to;
		to = SortedListEnd(src);
		iter = SortedListEnd(dest);

		DListSplice(from.internal_iter, to.internal_iter, 
		iter.internal_iter);
	}
}


