/******************************************************************
* File Name: uid_test.c											  *
* Name: Uriel Naiman											  *
* Create Date: 25.6.2020										  *
* Reviewed by: Anna Poriadkov 				 					  *
* Topic: UID Test file											  *
*******************************************************************/
#include <stdio.h>
#include <sys/types.h> /* pid_t */ 
#include <unistd.h>    /* pid_t */
#include <time.h>      /* size_t, time_t */
#include <assert.h>		/* assert */

#include "uid.h"

void TestCreate(void);
void TestIsSame(void);
void TestGetBadUID(void);

int main(void)
{
	printf(" ****** API unit testing: ******\n");
	
	printf("------------------test--create----------------------\n");
	TestCreate();

	printf("------------------test--is--same------------------\n");	
	TestIsSame();

	printf("------------------test--get--bad------------------\n");	
	TestGetBadUID();
	
	return (0);
}
	
	
	
static void PrintUID(unique_id_t uid)
{
	printf("counter = %lu\n", uid.counter);
	printf("pid = %u\n", uid.pid);
	printf("time = %ld\n", uid.time);
	printf("\n");
}

void TestCreate(void)
{
	unique_id_t uid = UIDCreate();
	
	assert(!UIDIsSame(uid, UIDGetBadUID()));
	PrintUID(uid);
	printf("all tests working well!!!\n");
}

void TestIsSame(void)
{
	unique_id_t uid1 = UIDCreate();
	unique_id_t uid2 = UIDCreate();
	PrintUID(uid1);
	PrintUID(uid2);
	
	printf("are uid's same? %d\n", UIDIsSame(uid1, uid2));
}

void TestGetBadUID(void)
{
	printf(" bad UID:\n");
	PrintUID(UIDGetBadUID());
}
