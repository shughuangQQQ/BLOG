#include"pthread_task_deal.h"
#include<errno.h>
void *add_task(pthread_pool_data* m_pthread_pool,int client_fd)
{
	printf("prepare to add client task\n");
	if(pthread_mutex_lock(&m_pthread_pool->thread_pool_data_lock))
		printf("lock error %d\n",errno);	
	if(m_pthread_pool->task_num==MAX_TASK_NUM)
	{
		printf("task queue full\n");
		pthread_cond_wait(&m_pthread_pool->task_queue_not_full_lock,&m_pthread_pool->thread_pool_data_lock);


	}
		m_pthread_pool->tasks[m_pthread_pool->task_queue_tail].task_fun=analysedata;
		m_pthread_pool->tasks[m_pthread_pool->task_queue_tail].fd=client_fd;

		m_pthread_pool->task_queue_tail=(m_pthread_pool->task_queue_tail+1)%MAX_TASK_NUM;
		m_pthread_pool->task_num++;

	if(pthread_cond_signal(&(m_pthread_pool->task_queue_not_empty_lock)))
		printf("lock error %d\n",errno);	
	if(pthread_mutex_unlock(&m_pthread_pool->thread_pool_data_lock))
		printf("lock error %d\n",errno);	

}

void pthread_pool_destory(pthread_pool_data* m_pool)
{

	if(pthread_mutex_lock(&m_pool->thread_pool_data_lock))
		printf("lock error %d\n",errno);	
	m_pool->exit_thread=MAX_PTHREAD_NUM;
	int eixt_num=m_pool->live_thread;
	if(pthread_mutex_unlock(&m_pool->thread_pool_data_lock))
		printf("lock error %d\n",errno);	

	m_pool->is_shutdown=true;
	pthread_join(m_pool->tids[MIN_PTHREAD_NUM-1],NULL);
	if(pthread_cond_broadcast(&m_pool->task_queue_not_empty_lock))
		printf("lock error %d\n",errno);	
	sleep(SLEEP_TIME);
	int i;
	for(i=0;i<MIN_PTHREAD_NUM;i++)
	{
		pthread_join(m_pool->tids[0],NULL);

	}
	if(pthread_mutex_lock(&m_pool->thread_pool_data_lock))
		printf("des error %d\n",errno);	

	if(pthread_mutex_destroy(&m_pool->thread_pool_data_lock))
		printf("des error %d\n",errno);	
	
	if(pthread_mutex_lock(&m_pool->thread_pool_count_lock))
		printf("des error %d\n",errno);	
	if(pthread_mutex_destroy(&m_pool->thread_pool_count_lock))
		printf("des error %d\n",errno);	
	if(pthread_cond_destroy(&m_pool->task_queue_not_empty_lock))
		printf("des  error %d\n",errno);	
	if(pthread_cond_destroy(&m_pool->task_queue_not_full_lock))
		printf("des error %d\n",errno);	

	free(m_pool->tids);
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
	if(pthread_mutex_lock(&m_pool->thread_pool_count_lock))
		printf("lock error%d\n",errno);
	m_pool->live_thread++;
	if(pthread_mutex_unlock(&m_pool->thread_pool_count_lock))
		printf("lock error%d\n",errno);
	while(true)
	{
		if(pthread_mutex_lock(&m_pool->thread_pool_data_lock))
		printf("lock error%d\n",errno);
		if(pthread_cond_wait(&m_pool->task_queue_not_empty_lock,&m_pool->thread_pool_data_lock))
		printf("lock error%d\n",errno);

		if(m_pool->exit_thread)
		{
			printf("not busy thread exit\n");
			m_pool->exit_thread--;
			m_pool->live_thread--;
			if(pthread_mutex_unlock(&m_pool->thread_pool_data_lock))
			printf("lock error%d\n",errno);
			pthread_exit(NULL);


		}
		if(m_pool->is_shutdown==true)
		{
			printf(" thread shut down\n");
			m_pool->live_thread--;
			if(pthread_mutex_unlock(&m_pool->thread_pool_data_lock))
		printf("lock error%d\n",errno);
			pthread_exit(NULL);
		}
		if(m_pool->task_num>0)
		{
		printf(" thread deal with task\n");
		m_pool->task_num--;
		newtask.task_fun=m_pool->tasks[m_pool->task_queue_front].task_fun;
		newtask.fd=m_pool->tasks[m_pool->task_queue_front].fd;
		m_pool->task_queue_front=(m_pool->task_queue_front+1)%MAX_TASK_NUM;
		if(pthread_cond_broadcast(&m_pool->task_queue_not_full_lock))
		printf("lock error%d\n",errno);
		if(pthread_mutex_unlock(&m_pool->thread_pool_data_lock))
		printf("lock error%d\n",errno);
		if(pthread_mutex_lock(&m_pool->thread_pool_count_lock))
		printf("lock error%d\n",errno);
		m_pool->busy_thread++;
		if(pthread_mutex_unlock(&m_pool->thread_pool_count_lock))
		printf("lock error%d\n",errno);
		(*(newtask.task_fun))(newtask.fd);
		if(pthread_mutex_lock(&m_pool->thread_pool_count_lock))
		printf("lock error%d\n",errno);
		m_pool->busy_thread--;
		if(pthread_mutex_unlock(&m_pool->thread_pool_count_lock))
		printf("lock error%d\n",errno);
		}
		else
		{
			if(pthread_mutex_unlock(&m_pool->thread_pool_data_lock))
				printf("lock error%d\n",errno);
		}




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
		sleep(SCANNER_PTHREAD_POOL_TIME_ADJUST);
		if(pthread_mutex_lock(&m_pool->thread_pool_data_lock))
		printf("lock error%d\n",errno);
		if(m_pool->is_shutdown==true)
		{
			printf("adjust thread:is_shutdown==true\n");
			pthread_mutex_unlock(&m_pool->thread_pool_data_lock);
			pthread_exit(NULL);
		}
		live_thread_num=m_pool->live_thread;
		queue_size=m_pool->task_num;
		if(pthread_mutex_unlock(&m_pool->thread_pool_data_lock))
		printf("lock error%d\n",errno);
		if(pthread_mutex_lock(&m_pool->thread_pool_count_lock))
		printf("lock error%d\n",errno);
		busy_thread_num=m_pool->busy_thread;
		if(pthread_mutex_unlock(&m_pool->thread_pool_count_lock))
		printf("lock error%d\n",errno);


		int add;
		if(live_thread_num*0.8<busy_thread_num&&live_thread_num<MAX_PTHREAD_NUM)
		{
			if(live_thread_num>MAX_PTHREAD_NUM-DEFAULT_ADD)
			{
				printf("thread is full,cant add more\n");
				continue;
			}
			else
			{
				add=DEFAULT_ADD;
				int i,g;
				printf("server busy &&create new thread\n");
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
				}

			}
			continue;
		}

		/*pthread_mutex_lock(&m_pool->thread_pool_data_lock);

		live_thread_num=m_pool->live_thread;
		queue_size=m_pool->task_num;
		pthread_mutex_unlock(&m_pool->thread_pool_data_lock);
		pthread_mutex_lock(&m_pool->thread_pool_count_lock);
		busy_thread_num=m_pool->busy_thread;
		pthread_mutex_unlock(&m_pool->thread_pool_count_lock);*/
		else if(live_thread_num>busy_thread_num*2&&live_thread_num>MIN_PTHREAD_NUM)
		{

			if(pthread_mutex_lock(&m_pool->thread_pool_data_lock))
		printf("lock error%d\n",errno);
			m_pool->exit_thread+=DEFAULT_ADD;

		if(	pthread_mutex_unlock(&m_pool->thread_pool_data_lock))
		printf("lock error%d\n",errno);

			for(h=0;h<DEFAULT_ADD;h++)
			{
				pthread_cond_signal(&m_pool->task_queue_not_empty_lock);
			}

		}
	}
}


pthread_pool_data *create_pthread_pool()
{
	//creat &&init thread struct(default num pthread)
	pthread_pool_data*m_pthread_pool=(pthread_pool_data*)malloc(sizeof(pthread_pool_data));	
	if(NULL==m_pthread_pool)
	{
		printf("malloc failed pthreadpool%d\n",errno);
	}
	m_pthread_pool->tids=(pthread_t*)malloc(sizeof(pthread_t)*MAX_PTHREAD_NUM);
	if(NULL==m_pthread_pool->tids)
	{
		printf("malloc pid failed\n");
	}
	memset(m_pthread_pool->tids,0,sizeof(m_pthread_pool->tids));
	//m_pthread_pool->tasks=(task_queue*)malloc(sizeof(task_queue)*MAX_TASK_NUM);
	/*if(m_pthread_pool->tasks)
	  {
	  printf("malloc task error ",errno);
	  }*/
	//memset(m_pthread_pool->tasks,0,sizeof(m_pthread_pool->tasks));
	if(0!=pthread_mutex_init(&m_pthread_pool->thread_pool_count_lock,NULL))
		printf("init thread_pool_count_lock failed:%d\n",errno);	
	if(0!=pthread_cond_init(&m_pthread_pool->task_queue_not_empty_lock,NULL))
		printf("init task_queue_not_empty_lock failed:%d\n",errno);	
	if(0!=pthread_cond_init(&m_pthread_pool->task_queue_not_full_lock,NULL))	
		printf("init task_queue_not_full_lock failed:%d\n",errno);	
	if(0!=pthread_mutex_init(&m_pthread_pool->thread_pool_data_lock,NULL))
		printf("init thread_pool_data_lock failed:%d\n",errno);	
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
		if(i==MIN_PTHREAD_NUM-1)
			if(0!=pthread_create(&m_pthread_pool->tids[i],0,pthread_adjust_fun,(void *)m_pthread_pool))
				printf("create thread pthread_adjust_fun error %d\n",errno);
			else
			{
				if(0!=pthread_create(&m_pthread_pool->tids[i],0,pthread_fun,(void *)m_pthread_pool))
					printf("create pthread error %d\n",errno);
			}


	}
	return  m_pthread_pool;



}

