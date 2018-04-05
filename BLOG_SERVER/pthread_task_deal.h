#ifndef PTHREAD_CLASS
#define PTHREAD_CLASS
#include"server_data_analy.h"
#include"PAC_INET.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>
#include<signal.h>
#define SLEEP_TIME 10
#define SCANNER_PTHREAD_POOL_TIME 10
#define MIN_PTHREAD_NUM 5
#define MAX_PTHREAD_NUM 100
#define MAX_TASK_NUM 100
#define DEFAULT_ADD 5
typedef struct node7
{
	void * (*task_fun)(int client_fd);
	int fd;
}task_queue;

typedef struct node6
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
void *pthread_fun(void *argv);
int is_ALIVE(pid_t tid);
void pthread_pool_destory(pthread_pool_data* m_pool);
void *pthread_adjust_fun(void *argv);
pthread_pool_data *create_pthread_pool();
void *add_task(pthread_pool_data* m_pthread_pool,int client_fd);


void *add_task(pthread_pool_data* m_pthread_pool,int client_fd)
{
	printf("prepare to add client task\n");
	pthread_mutex_lock(&m_pthread_pool->thread_pool_data_lock);
	if(m_pthread_pool->task_num==MAX_TASK_NUM)
	{
		pthread_cond_wait(&m_pthread_pool->task_queue_not_full_lock,&m_pthread_pool->thread_pool_data_lock);


	}
	else 
	{
		m_pthread_pool->tasks[m_pthread_pool->task_queue_tail].task_fun=analysedata;
		m_pthread_pool->tasks[m_pthread_pool->task_queue_tail].fd=client_fd;

		m_pthread_pool->task_queue_tail=(m_pthread_pool->task_queue_tail+1)%MAX_TASK_NUM;
		m_pthread_pool->task_num++;

	}


	pthread_cond_signal(&(m_pthread_pool->task_queue_not_empty_lock));
	pthread_mutex_unlock(&m_pthread_pool->thread_pool_data_lock);

}

void pthread_pool_destory(pthread_pool_data* m_pool)
{

	pthread_mutex_lock(&m_pool->thread_pool_data_lock);
	m_pool->exit_thread=MAX_PTHREAD_NUM;
	int eixt_num=m_pool->live_thread;
	pthread_mutex_unlock(&m_pool->thread_pool_data_lock);
	
	m_pool->is_shutdown=true;
	pthread_join(m_pool->tids[0],NULL);
	pthread_cond_broadcast(&m_pool->task_queue_not_empty_lock);
	sleep(SLEEP_TIME);
	int i;
	for(i=0;i<MIN_PTHREAD_NUM;i++)
	{
		pthread_join(m_pool->tids[0],NULL);

	}
	pthread_mutex_lock(&m_pool->thread_pool_data_lock);
	pthread_mutex_destroy(&m_pool->thread_pool_data_lock);
	pthread_mutex_lock(&m_pool->thread_pool_count_lock);
	pthread_mutex_destroy(&m_pool->thread_pool_count_lock);
	pthread_cond_destroy(&m_pool->task_queue_not_empty_lock);
	pthread_cond_destroy(&m_pool->task_queue_not_full_lock);

	free(m_pool->tids);
	free(m_pool->tasks);
	free(m_pool);

}
int is_ALIVE(pid_t tid)
{

	int kill_rc = pthread_kill(tid, 0);
	if (kill_rc == ESRCH)
		return false;
	else
	return true;
}
void *pthread_fun(void *argv)
{
	pthread_pool_data* m_pool=(pthread_pool_data*)argv;
	task_queue newtask;
	int wait_exit_thread;
	pthread_mutex_lock(&m_pool->thread_pool_count_lock);
	m_pool->live_thread++;
	pthread_mutex_unlock(&m_pool->thread_pool_count_lock);
	while(true)
	{
		pthread_mutex_lock(&m_pool->thread_pool_data_lock);
		


		pthread_cond_wait(&m_pool->task_queue_not_empty_lock,&m_pool->thread_pool_data_lock);

		if(m_pool->exit_thread)
		{
			m_pool->exit_thread--;
			m_pool->live_thread--;
			pthread_mutex_unlock(&m_pool->thread_pool_data_lock);
			pthread_exit(NULL);


		}


		
		if(m_pool->is_shutdown==true)
		{
			m_pool->live_thread--;
			pthread_mutex_unlock(&m_pool->thread_pool_data_lock);
			pthread_exit(NULL);
		}

		m_pool->task_num--;
		m_pool->task_queue_front=(m_pool->task_queue_front+1)%MAX_TASK_NUM;
		newtask.task_fun=m_pool->tasks[m_pool->task_queue_tail].task_fun;
		newtask.fd=m_pool->tasks[m_pool->task_queue_tail].fd;
		pthread_cond_broadcast(&m_pool->task_queue_not_full_lock);
		pthread_mutex_lock(&m_pool->thread_pool_count_lock);
		m_pool->busy_thread++;
		pthread_mutex_unlock(&m_pool->thread_pool_count_lock);
		pthread_mutex_unlock(&m_pool->thread_pool_data_lock);
		(*(newtask.task_fun))(newtask.fd);
		pthread_mutex_lock(&m_pool->thread_pool_count_lock);
		m_pool->busy_thread--;
		pthread_mutex_unlock(&m_pool->thread_pool_count_lock);





	}
	
}

void *pthread_adjust_fun(void *argv)
{
	pthread_pool_data* m_pool=(pthread_pool_data*)argv;
	int live_thread_num;
	int queue_size;
	int busy_thread_num;
	int h;
	while (true)
	{
		sleep(SCANNER_PTHREAD_POOL_TIME);
		pthread_mutex_lock(&m_pool->thread_pool_data_lock);
		if(m_pool->is_shutdown==true)
		{
			pthread_mutex_unlock(&m_pool->thread_pool_data_lock);
			pthread_exit(NULL);
		}
		live_thread_num=m_pool->live_thread;
		queue_size=m_pool->task_num;
		pthread_mutex_unlock(&m_pool->thread_pool_data_lock);
		pthread_mutex_lock(&m_pool->thread_pool_count_lock);
		busy_thread_num=m_pool->busy_thread;
		pthread_mutex_unlock(&m_pool->thread_pool_count_lock);


		int add;
		if(live_thread_num*0.8<busy_thread_num&&live_thread_num<MAX_PTHREAD_NUM)
		{
			if(live_thread_num<MAX_PTHREAD_NUM-DEFAULT_ADD)
			{
				add=DEFAULT_ADD;
			}
			else
			{
				printf("thread is full");

			}
			pthread_mutex_lock(&m_pool->thread_pool_data_lock);
			int i,g;
			for(i=0,g=0;i<MAX_PTHREAD_NUM&&g<add;i++)
			{
				if(is_ALIVE(m_pool->tids[i]))
					continue;
				else if(m_pool->tids[i]==0||!is_ALIVE(m_pool->tids[i]))
				{
					if(pthread_create(&m_pool->tids[i],0,pthread_fun,(void *)m_pool))
					{
						printf("extend pthread failed\n");
					}
				}


				m_pool->live_thread+=DEFAULT_ADD;


			}
			pthread_mutex_unlock(&m_pool->thread_pool_data_lock);
			continue;
		}

		pthread_mutex_lock(&m_pool->thread_pool_data_lock);

		live_thread_num=m_pool->live_thread;
		queue_size=m_pool->task_num;
		pthread_mutex_unlock(&m_pool->thread_pool_data_lock);
		pthread_mutex_lock(&m_pool->thread_pool_count_lock);
		busy_thread_num=m_pool->busy_thread;
		pthread_mutex_unlock(&m_pool->thread_pool_count_lock);
		if(live_thread_num>busy_thread_num*2&&live_thread_num>MIN_PTHREAD_NUM)
		{
			
			pthread_mutex_lock(&m_pool->thread_pool_data_lock);
			m_pool->exit_thread+=DEFAULT_ADD;

			pthread_mutex_unlock(&m_pool->thread_pool_data_lock);

			for(h=0;h<DEFAULT_ADD;h++)
			{
				pthread_cond_signal(&m_pool->task_queue_not_full_lock);
			}

		}
	}
}


pthread_pool_data *create_pthread_pool()
{
	pthread_pool_data*m_pthread_pool=(pthread_pool_data*)malloc(sizeof(pthread_pool_data));	
	if(NULL==m_pthread_pool)
	{
		printf("malloc failed\n");
	}
	m_pthread_pool->tids=(pthread_t*)malloc(sizeof(pthread_t)*MAX_PTHREAD_NUM);
		if(NULL==m_pthread_pool->tids)
	{
		printf("malloc failed\n");
	}
	memset(m_pthread_pool->tids,0,sizeof(m_pthread_pool->tids));
	m_pthread_pool->tasks=(task_queue*)malloc(sizeof(task_queue)*MAX_TASK_NUM);
	if(m_pthread_pool->tasks)
	{
		printf("malloc error\n");
	}
	memset(m_pthread_pool->tasks,0,sizeof(m_pthread_pool->tasks));
	pthread_mutex_init(&m_pthread_pool->thread_pool_count_lock,NULL);
	pthread_cond_init(&m_pthread_pool->task_queue_not_empty_lock,NULL);
	pthread_cond_init(&m_pthread_pool->task_queue_not_full_lock,NULL);
	pthread_mutex_init(&m_pthread_pool->thread_pool_data_lock,NULL);
	m_pthread_pool->busy_thread=0;
	m_pthread_pool->is_shutdown=false;
	m_pthread_pool->live_thread=0;
	m_pthread_pool->exit_thread=0;
	m_pthread_pool->task_queue_front=0;
	m_pthread_pool->task_queue_tail=0;
	m_pthread_pool->task_num=0;
	
	int i;
	for(i=0;i<MIN_PTHREAD_NUM;i++)
	{
		if(i==0)
			if(pthread_create(&m_pthread_pool->tids[i],0,pthread_adjust_fun,(void *)m_pthread_pool))
			{
				printf("create thread pthread_adjust_fun error\n");
			}
		else
		{
			if(pthread_create(&m_pthread_pool->tids[i],0,pthread_fun,(void *)m_pthread_pool))
			{
				printf("create pthread error \n");
			}

		}

	//todo deal with error;

	}
	return  m_pthread_pool;


	
}
#endif

