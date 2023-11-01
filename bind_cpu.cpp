#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/sysinfo.h>
#include<unistd.h>

#define __USE_GNU
#include<sched.h>
#include<ctype.h>
#include<string.h>
#include<pthread.h>
#define THREAD_MAX_NUM 200  

int num=0;  
void* threadFun(void* arg)  
{
         cpu_set_t mask;  
         cpu_set_t get;   
         int *a = (int *)arg; 
         int i;

         printf("the thread is:%d\n",*a);  
         CPU_ZERO(&mask);    
         CPU_SET(*a,&mask);   
         if (sched_setaffinity(0, sizeof(mask), &mask) == -1)
         {
                   printf("warning: could not set CPU affinity, continuing...\n");
         }

           CPU_ZERO(&get);
           if (sched_getaffinity(0, sizeof(get), &get) == -1)
           {
                    printf("warning: cound not get thread affinity, continuing...\n");
           }
           for (i = 0; i < num; i++)
           {
                    if (CPU_ISSET(i, &get))
                    {
                             printf("this thread %d is running processor : %d\n", i,i);
                    }
           }

         return NULL;
}

int main(int argc, char* argv[])
{
         int tid[THREAD_MAX_NUM];
         int i;
         pthread_t thread[THREAD_MAX_NUM];

         num = sysconf(_SC_NPROCESSORS_CONF);  
         if (num > THREAD_MAX_NUM) {
            printf("num of cores[%d] is bigger than THREAD_MAX_NUM[%d]!\n", num, THREAD_MAX_NUM);
            return -1;
         }
         printf("system has %i processor(s). \n", num);

         for(i=0;i<num;i++)
         {
                   tid[i] = i;  
                   pthread_create(&thread[i],NULL,threadFun,(void*)&tid[i]);
         }
         for(i=0; i< num; i++)
         {
                   pthread_join(thread[i],NULL);
         }
         return 0;
}
