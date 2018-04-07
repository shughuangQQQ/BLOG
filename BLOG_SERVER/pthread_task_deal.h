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
#define SCANNER_PTHREAD_POOL_TIME_ADJUST 10
#define MIN_PTHREAD_NUM 5
#define MAX_PTHREAD_NUM 100
#define MAX_TASK_NUM 100
#define DEFAULT_ADD 5
typedef struct node7
{
	void * (*task_fun)(int,int);
	int fd;
}task_queue;

typedef struct node6
{
	pthread_t *tids;
	task_queue tasks[MAX_TASK_NUM];
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
	int m_epollfd;
}pthread_pool_data;
void *pthread_fun(void *argv);
int is_ALIVE(pid_t tid);
void pthread_pool_destory(pthread_pool_data* m_pool);
void *pthread_adjust_fun(void *argv);
pthread_pool_data *create_pthread_pool(int);
void *add_task(pthread_pool_data* m_pthread_pool,int client_fd);

#endif
