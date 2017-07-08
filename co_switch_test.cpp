#include <stdio.h>
#include <sys/time.h>
#include "co_routine.h"

static unsigned int sw_times=0;
static struct timeval st = { 0 };
static struct timeval et = { 0 };

void* func(void* args)
{
	while(true){
        sw_times++;
        //printf("start\n");
        if(sw_times==1000000){
            gettimeofday(&et,NULL);  
            printf("us:%d\n",(et.tv_sec-st.tv_sec)*1000000+(et.tv_usec-st.tv_usec));          
        }
    	co_yield_ct();
    }
}
int main()
{
    printf("start\n");
	stCoRoutine_t* func_routine;
	co_create(&func_routine, NULL, func, NULL);
    stCoRoutine_t* func_routine2;
	co_create(&func_routine2, NULL, func, NULL);
    gettimeofday(&st,NULL);
    while(1){
	    co_resume(func_routine);
        co_resume(func_routine2);
    }

	co_eventloop(co_get_epoll_ct(), NULL, NULL);
	return 0;
}
