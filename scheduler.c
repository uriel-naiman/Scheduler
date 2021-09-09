/*****************************************************************
* File Name: scheduler.c									  *
* Name: Uriel Naiman										  *
* Create Date: 29.6.2020									  *
* Reviewed by: Guy Barazani									  *
* Topic: scheduler C file									  *
*******************************************************************/

#include <stdlib.h> 		/* malloc, free */
#include <assert.h>			/* assert */
#include <time.h>			/* time_t, size_t*/

#include "scheduler.h"
#include "uid.h"
#include "pqueue.h"

/* scheduler handler*/
struct scheduler
{
	pqueue_t *pq;		/* priority queue to schedule timers */
	int stop_flag;		/* flag to use in run loop, indication if 
						stop function has been used */
};

/* timer struct- to be passed into scheduler as data pointer */
typedef struct timer
{
	unique_id_t uid;		/* unique timer id */
	time_t initial_time;	/* interval recieved from user */
	time_t interval;		/* time to sleep after prev timer */
	sch_task_func_t task;	/* task to be performed by timer */
	void *param;			/* parameter to be passed to task */
}sch_timer_t;


/* util to free timer struct */                                    
static void FreeTimer(sch_timer_t *tmr);

/* cmp func to pass to pq for list linup */                        
int cmp(const void *data1, const void *data2, void *param);

/* ismatch function to pass to pqerase to find uid to remove */
static int IsMatch(const void *data1, const void *data2, void *param);

/* function used in start and in run- for reseting timer and enqueueing */
static int PushTimer(sch_t *sch, sch_timer_t *timer);

/*----------------------------------------------------------------*/

sch_t *SchCreate(void)
{
	sch_t *sch = (sch_t*) malloc(sizeof(sch_t));
	
	if (NULL != sch)
	{ 
		sch->stop_flag = 0;
		sch->pq = PQCreate(cmp, NULL);
		
		if (NULL == sch->pq)
		{
			free(sch);
			sch = NULL;
		}
	}
	
	return (sch);
}

/*----------------------------------------------------------------*/

void SchDestroy(sch_t *sch)
{
	sch_timer_t *temp = NULL;
	
	assert(sch);
	
	while (!PQIsEmpty(sch->pq))
	{
		temp = PQDequeue(sch->pq);
		free(temp);
		temp = NULL;
	}
	
	PQDestroy(sch->pq);
	
	free(sch);
	sch = NULL;
}

/*----------------------------------------------------------------*/

int SchRun(sch_t *sch)
{
	sch_timer_t *timer = NULL;
	int sleep_time = 0;
	int status = 0;
	
	assert(sch);
	
	/* set initial sleep until first timer */
	timer = (sch_timer_t*)PQPeek(sch->pq);
	sleep_time = (int)(timer->initial_time);
	sleep (sleep_time);
	
	/* as long as PQ has more elements dequeue them 
		and perform the tast given to them */
	while (!PQIsEmpty(sch->pq) && (!sch->stop_flag))
	{
		timer = PQDequeue(sch->pq);
		status = timer->task(sch, timer->uid, NULL);
		
	/* indications for reactivate, cancel timer or stop 'sch' */
		if (1 == status)
		{
			PushTimer(sch, timer);
		}
		if (-1 == status)
		{
			SchStop(sch);
			return (0);
		}
		if (0 == status)
		{
			FreeTimer(timer);
		}
		
	/* set new time to sleep according to PQ 
			first element, as long as PQ is not Empty */
		if(!PQIsEmpty(sch->pq))
		{
			timer = (sch_timer_t*)PQPeek(sch->pq);
			sleep_time = (int)(timer->interval - time(NULL));
			sleep (sleep_time);
		}
	}
		
		return (0);
}
/*----------------------------------------------------------------*/

int SchStop(sch_t *sch)
{
	sch_timer_t *temp = NULL;
	
	sch->stop_flag = 1;

	/* because need to free all timers, cant use PQClear, 
	rather than pop each timer and release allocated memory */
	while(!PQIsEmpty(sch->pq))
	{
		temp = PQDequeue(sch->pq);
		free(temp);
		temp = NULL;
	}
	
	return (PQIsEmpty(sch->pq));
}

/*----------------------------------------------------------------*/

unique_id_t SchTimerStart(sch_t *sch, 
                                size_t interval_in_sec,
                                sch_task_func_t task,
                                void* param)
{
	sch_timer_t *timer = NULL;

	assert(sch);
	
	/* allocate memory for timer pointer */
	timer = (sch_timer_t*) malloc(sizeof(sch_timer_t));
	if (NULL == timer)
	{
		return (UIDGetBadUID());
	}
	
	/* fill timer pointer fields */
	timer->uid = UIDCreate();
	timer->initial_time = interval_in_sec;
	timer->task = task;
	timer->param = param;
	/* add timer info into PQ and check if successful */
	if (PushTimer(sch, timer))
	{
		return (UIDGetBadUID());
	}
	
	return (timer->uid);
}
	

/*----------------------------------------------------------------*/

void SchTimerCancel(sch_t *sch, unique_id_t uid)
{
	sch_timer_t *temp = NULL;
	
	assert(sch);
	
	temp = PQErase(sch->pq, &uid, IsMatch, NULL);
	
	FreeTimer(temp);
}	

/*----------------------------------------------------------------*/

static void FreeTimer(sch_timer_t *tmr)
{
	free(tmr);
	tmr = NULL;
}

/*----------------------------------------------------------------*/

/* function to pass to PQ for indication of priority settings */
int cmp(const void *data1, const void *data2, void *param)
{
	const sch_timer_t *timer1 = data1;
	const sch_timer_t *timer2 = data2;
	
	if (timer1->interval > timer2->interval)
	{
		return (1);
	}
	else if (timer1->interval < timer2->interval)
	{
		return (-1);
	}
	(void)(param);
	return (0);
}

/*----------------------------------------------------------------*/

/* function to be called by from PQ when looking for maching uid */
static int IsMatch(const void *data1, const void *data2, void *param)
{
	unique_id_t uid1 = ((sch_timer_t *)data1)->uid;
    unique_id_t uid2 = *(unique_id_t *)data2;

	(void)(param);
    return (UIDIsSame(uid1,uid2));
}

/* function used in start and in run- for reseting timer and enqueueing */
static int PushTimer(sch_t *sch, sch_timer_t *timer)
{
	timer->interval = time(NULL) + timer->initial_time;
	
	if (0 == PQEnqueue(sch->pq, timer))
	{
		FreeTimer(timer);
		return (1);
	}
	return (0);
}

