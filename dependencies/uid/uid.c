/******************************************************************
* File Name: uid.c												  *
* Name: Uriel Naiman											  *
* Create Date: 25.6.2020										  *
* Reviewed by: Anna Poriadkov 				 					  *
* Topic: UID C file										  *
*******************************************************************/
#include <stdio.h>
#include <sys/types.h> /* pid_t */ 
#include <unistd.h>    /* pid_t */
#include <time.h>      /* size_t, time_t */

#include "uid.h"

#define FAIL -1

/* UidCreate:
* - Creates a new UID.
*
* Return value:
*   - if succeeded, returns a new UID 
*	- if failed, returns a bad UID
*
* Comments: 
*	- User should compare the return value to the bad ID to determine
*	  if function succeeded.
*/
unique_id_t UIDCreate(void)
{
	unique_id_t uid = {0};
	static size_t counter = 0;
	
	uid.counter = ++counter;
	uid.pid = getpid();
	uid.time = time(NULL);
	
	if (FAIL == uid.time)
	{
		return (UIDGetBadUID());
	}
	
	return(uid);
}

/*---------------------------------------------------------------------------*/

int UIDIsSame(unique_id_t uid1, unique_id_t uid2)
{
	return (uid1.counter == uid2.counter &&
				 uid1.pid == uid2.pid && 
				 uid1.time == uid2.time);
}

/*---------------------------------------------------------------------------*/

unique_id_t UIDGetBadUID(void)
{
	unique_id_t uid = {0};
	uid.counter = 0;
	uid.pid = -1;
	uid. time = 0;
	
	return (uid);
}


