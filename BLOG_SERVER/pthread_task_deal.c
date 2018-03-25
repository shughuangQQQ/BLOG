#ifndef PTHREAD_CLASS
#define PTHREAD_CLASS


#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#define MIN_PTHREAD_NUM 5
#define MAX_PTHREAD_NUM 100
#define MAX_TASK_NUM 100

typedef struct node
{
	pthread_t *tids;
	task_queue *tasks;
	pthread_cond_t task_queue_not_empty_lock;
	pthread_cond_t task_queue_not_full_lock;
	pthread_mutex_t thread_pool_data_lock;
	int busy_thread;
	int live_thread;
	int exit_thread;



}pthread_pool_data;
typedef struct node2
{
	void * (*task_fun)(void *);
	void *argv;

}task_queue;
void *pthread_fun(void *argv)
{
	pthread_pool_data* m_pool=(pthread_pool_data*)argv;
}
void *create_pthread_pool()
{
	pthread_pool_data*m_pthread_pool=(pthread_pool_data*)malloc(sizeof(pthread_pool_data));	
	m_pthread_pool->tids=(pthread_t*)malloc(sizeof(pthread_t)*MAX_PTHREAD_NUM);\
	memset(m_pthread_pool->tids,0,sizeof(m_pthread_pool->tids));
	m_pthread_pool->tasks=(task_queue*)malloc(sizeof(task_queue)*MAX_TASK_NUM);
	memset(m_pthread_pool->tasks,0,sizeof(m_pthread_pool->tasks));
	pthread_cond_init(&m_pthread_pool->task_queue_not_empty_lock);
	pthread_cond_init(&m_pthread_pool->task_queue_not_full_lock);
	pthread_mutex_init(&m_pthread_pool->thread_pool_data_lock);
	m_pthread_pool->busy_thread=0;
	m_pthread_pool->live_thread=MIN_PTHREAD_NUM;
	m_pthread_pool->exit_thread=0;
	
	int i;
	for(i=0;i<MIN_PTHREAD_NUML;i++)
	{
	pthread_create(m_pthread_pool->tids[i],0,pthread_fun,(void *)m_pthread_pool);	
		
	}


	
}



#endif

