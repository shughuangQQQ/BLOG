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
	pthread_mutex_t thread_pool_count_lock;
	int busy_thread;
	int live_thread;
	int exit_thread;
	int task_queue_tail;
	int task_queue_front;
	int task_num;
	int is_shutdown;
}pthread_pool_data;
typedef struct node2
{
	void * (*task_fun)(int client_fd);
	int fd;

}task_queue;
void *pthread_fun(void *argv)
{
	pthread_pool_data* m_pool=(pthread_pool_data*)argv;
	task_queue newtask;

	
	while(true)
	{
		pthread_mutex_lock(&m_pool->thread_pool_data_lock);
		if(m_pool->task_num==0)
		{
			pthread_cond_wait(&mpool->task_queue_not_empty_lock,&thread_pool_data_lock);
		}
		if(m_pool->is_shutdown==true)
		{
			pthread_mutex_unlock(&m_pool->pthread_pool_data);
			pthread_exit(NULL);
		}
			
			m_pool->task_num--;
			m_pool->task_queue_front=(m_pool->task_queue_front+1)%MAX_TASK_NUM;
			newtask.task_fun=m_pool->tasks[task_queue_tail];
			newtask.fd=m_pool->tasks[task_queue_tail].fd;
			pthread_cond_broadcast(&m_pool->task_queue_not_full_lock);
			pthread_mutex_lock(m_pool->thread_pool_count_lock);
			m_pool->busy_thread++;
			pthread_mutex_unlock(m_pool->thread_pool_count_lock);
			pthread_mutex_unlock(m_pool->thread_pool_data_lock);
			(*(newtask.task_fun))(newtask.fd);
			pthread_mutex_lock(m_pool->thread_pool_count_lock);
			m_pool->busy_thread--;
			pthread_mutex_unlock(m_pool->thread_pool_count_lock);





	}
	
}
void *pthread_adjust_fun(void *argv)
{
pthread_pool_data* m_pool=(pthread_pool_data*)argv;

}
pthread_pool_data *create_pthread_pool()
{
	pthread_pool_data*m_pthread_pool=(pthread_pool_data*)malloc(sizeof(pthread_pool_data));	
	m_pthread_pool->tids=(pthread_t*)malloc(sizeof(pthread_t)*MAX_PTHREAD_NUM);\
	memset(m_pthread_pool->tids,0,sizeof(m_pthread_pool->tids));
	m_pthread_pool->tasks=(task_queue*)malloc(sizeof(task_queue)*MAX_TASK_NUM);
	memset(m_pthread_pool->tasks,0,sizeof(m_pthread_pool->tasks));
	pthread_mutex_init(&m_pthread_pool->thread_pool_count_lock);
	pthread_cond_init(&m_pthread_pool->task_queue_not_empty_lock);
	pthread_cond_init(&m_pthread_pool->task_queue_not_full_lock);
	pthread_mutex_init(&m_pthread_pool->thread_pool_data_lock);
	m_pthread_pool->busy_thread=0;
	m_pthread_pool->is_shutdown=0;
	m_pthread_pool->live_thread=MIN_PTHREAD_NUM;
	m_pthread_pool->exit_thread=0;
	m_pthread_pool->task_queue_front=0;
	m_pthread_pool->task_queue_tail=0;
	m_pthread_pool->task_num=0;
	int i;
	for(i=0;i<MIN_PTHREAD_NUML;i++)
	{
	if(i==0)
	pthread_create(m_pthread_pool->tids[i],0,pthread_adjust_fun,(void *)m_pthread_pool);	
	else
	{
	pthread_create(m_pthread_pool->tids[i],0,pthread_fun,(void *)m_pthread_pool);

	}

	//todo deal with error;
	
	}
	return  m_pthread_pool;


	
}
void *analysedata(int client_fd)
{


}
void *add_task(pthread_pool_data* m_pthread_pool,int client_fd)
{
	pthread_mutex_lock(&m_pthread_pool->thread_pool_data_lock);
	if(m_pthread_pool->task_num==MAX_TASK_NUM)
	{
		pthread_cond_wait(&m_pthread_pool->task_queue_not_full_lock,&m_pthread_pool->thread_pool_data_lock);


	}
	else 
	{
		m_pthread_pool->tasks[task_queue_tail].task_fun=analysedata;
		m_pthread_pool->tasks[task_queue_tail].fd=client_fd;

		m_pthread_pool->task_queue_tail=(m_pthread_pool->task_queue_tail+1)%MAX_TASK_NUM;
		m_pthread_pool->task_num++;

	}


	pthread_cond_signal(&(m_pthread_pool->task_queue_not_empty_lock));
	pthread_mutex_unlock(&m_pthread_pool->thread_pool_data_lock);

}



#endif

