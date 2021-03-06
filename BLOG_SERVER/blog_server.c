#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<assert.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include<signal.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include"PAC_INET.h"
#include"pthread_task_deal.h"
#define MAX_LISTEN_NUM 1024
#define CHILD_PROCESS_NUM 1


void run_parent_process();
void add_sig(int);
void sig_action(int sig);
void run_child_process();
void setnonblocking(int fd);
static int m_pipe_parent[2];
typedef struct node
{
	pid_t pid;
	int pipe_conv[2];
}chilrd_process;
typedef struct node3
{

	chilrd_process process_all[CHILD_PROCESS_NUM];
	bool close_process;
	int m_index;
	int epoll_fd;
	int servers_fd;
	pthread_pool_data*child_process_pool;

}process_utils;
void creaete_process();
void add_epoll_fd(int fd);
process_utils p_util;



int main()
{



	p_util.close_process=true;
	struct sockaddr_in server_addr;

	int server_fd=socket(AF_INET,SOCK_STREAM,0);

	if(server_fd==-1)
	{
		printf("scoket_create failed  %d\n",errno);	
	}
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(SERVER_PORT);
	//server_addr.sin_addr.s_addr=htonl(SERVER_IP_1);
	inet_pton(AF_INET,SERVER_IP_1,&server_addr.sin_addr.s_addr);	
	if(-1==bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr)))
	{
		printf("sock_bind failed errno%d\n",errno);	
	}
	if(-1==listen(server_fd,LISTEN))
	{
		printf("listen failed\n");	
	}
	p_util.servers_fd=server_fd;

	creaete_process();
	if(p_util.m_index==-1)
	{

		p_util.epoll_fd=epoll_create(MAX_LISTEN_NUM);
		run_parent_process();	
	}
	else
	{

		p_util.epoll_fd=epoll_create(MAX_LISTEN_NUM);
		printf("child run m_index:%d\n",p_util.m_index);
		p_util.child_process_pool=create_pthread_pool(p_util.epoll_fd);
		run_child_process();	
	}



	return 0;

}
void add_epoll_fd(int fd)
{

	struct epoll_event tmp_event;
	tmp_event.data.fd=fd;
	tmp_event.events=EPOLLIN;
	if(-1==epoll_ctl(p_util.epoll_fd,EPOLL_CTL_ADD,fd,&tmp_event))
	{
		printf("epoll_add_failed\n");

	}
	setnonblocking(fd);
}
void run_parent_process()
{

	if(-1==socketpair(AF_UNIX,SOCK_STREAM,0,m_pipe_parent))
	{
		printf("create  parent  signal pipe error\n");	
	}
	add_epoll_fd(m_pipe_parent[0]);
	setnonblocking(m_pipe_parent[1]);
	add_sig(SIGCHLD);
	add_sig(SIGINT);
	add_sig(SIGPIPE);
	add_sig(SIGTERM);


	add_epoll_fd(p_util.servers_fd);


	int get_num,i;
	int chose_server=0;
	struct epoll_event all_event[MAX_LISTEN_NUM];
	int send_temp=999;
	int k;
	while(p_util.close_process)
	{
		sleep(5);
		get_num=epoll_wait(p_util.epoll_fd,all_event,MAX_LISTEN_NUM,-1);
		for(i=0;i<get_num;i++)
		{
			if(!all_event[i].events&EPOLLIN)
				continue;
			if(all_event[i].data.fd==p_util.servers_fd)
			{
				printf("parent recv con\n");
				size_t sendbuf=send(p_util.process_all[chose_server].pipe_conv[0],(char *)&send_temp,sizeof(send_temp),0);	
				if(sendbuf==-1)
				{
					printf("new connected send to child process failed\n");	
				}
				printf("main process recv a connect&&send to child pipe child:%d\n",chose_server);
				chose_server=(chose_server+1)%CHILD_PROCESS_NUM;
				continue;
			}
			else if(all_event[i].data.fd==m_pipe_parent[0])
			{

				char sig[1024];
				bzero(sig,1024);
				size_t temprecbuf=recv(m_pipe_parent[0],(char *)&sig,sizeof(sig),0);
				if(temprecbuf<=0)
					continue;
				else 
					printf("parent process recv signal  \n");
				for(k=0;k<temprecbuf;k++)
				{
					switch(sig[k])
					{
						case SIGCHLD:
							{
								pid_t e_pid;
								int stat;
								while((e_pid=waitpid(-1,&stat,WNOHANG))>0)
								{
									for(i=1;i<CHILD_PROCESS_NUM;i++)
									{
										if(e_pid==p_util.process_all[i].pid)
										{
											printf("close child process %d\n",e_pid);
											close(p_util.process_all[i].pipe_conv[0]);
											p_util.process_all[i].pid=-1;
										}

									}
								}
								p_util.close_process=false;



								break;
							}
						case SIGTERM:
							{
								printf("main process exit\n");	
								for(i=0;i<CHILD_PROCESS_NUM;i++)
								{
									if(p_util.process_all[i].pid!=-1)
									{
										kill(p_util.process_all[i].pid,SIGTERM);	
									}
								}
							}
							break;
						case SIGINT:
							{
								printf("close all server\n");
								for(i=0;i<CHILD_PROCESS_NUM;i++)
								{
									if(p_util.process_all[i].pid!=-1)
									{
										kill(p_util.process_all[i].pid,SIGTERM);	
									}
								}
							}
							break;
					}
					bzero(sig,1024);
				}
			}
			else
			{
				continue;	
			}


		}


	}

}
void run_child_process()
{

	int get_num,i;
	int chose_server=0;
	struct epoll_event all_event[MAX_LISTEN_NUM];
	int recv_temp;
	int client_fd;
	if( -1==socketpair(AF_UNIX,SOCK_STREAM,0,m_pipe_parent))
	{
		printf("childe process signal pipe create failed\n");

	}
	add_epoll_fd(m_pipe_parent[0]);
	add_epoll_fd(p_util.process_all[p_util.m_index].pipe_conv[1]);

	setnonblocking(m_pipe_parent[1]);
	add_sig(SIGCHLD/*,sig_action*/);
	add_sig(SIGINT);
	add_sig(SIGPIPE);
	add_sig(SIGTERM);

	struct sockaddr_in client_addr;
	socklen_t client_addr_size;
	while(p_util.close_process)
	{
		sleep(5);
		get_num=epoll_wait(p_util.epoll_fd,all_event,MAX_LISTEN_NUM,-1);
		for(i=0;i<get_num;i++)
		{
			/*if(all_event[i].data.fd==p_util.servers_fd)
			  {
			  printf("error child %d listen server_fd\n",p_util.m_index);	
			  }*/
			if(all_event[i].data.fd==p_util.process_all[p_util.m_index].pipe_conv[1])
			{
				size_t recbuf=recv(p_util.process_all[p_util.m_index].pipe_conv[1],(char *)&recv_temp,sizeof(recv_temp),0);	
				if(recbuf==-1)
				{
					printf("child process: new conected recv failed %d\n",errno);	
					continue;
				}
				client_fd=accept(p_util.servers_fd,(struct sockaddr*)&client_addr,&client_addr_size);
				if(-1==client_fd)
				{
					printf("recv client failed\n");	

					continue;
				}
				add_epoll_fd(client_fd);

			}

			else if(all_event[i].data.fd==m_pipe_parent[0]&&(all_event[i].events & EPOLLIN)) 
			{
				char signals[1024];
				int k,m;
				int temp_recv=recv(m_pipe_parent[0],signals,sizeof(signals),0);
				printf("child recv signal \n");
				if(temp_recv<=0)
				{
					continue;
				}
				else
				{

					for(k=0;k<temp_recv;k++)
					{
						switch(signals[k])
						{
							case SIGCHLD:
								{
									pid_t m_pid;
									int stat;
									while((m_pid=waitpid(-1,&stat,WNOHANG))>0)
									{
										continue;	
									}
									break;

								}
							case SIGTERM:
								{
									p_util.close_process=false;
									printf("child process exit\n");
									break;
								}
							case SIGINT:
								{
									p_util.close_process=false;

									break;
								}



						}


					}


				}

			}
			else if(all_event[i].events & EPOLLIN)
			{
				/*if(readlen==0)
				{
					printf("one client exit by accident\n");
					epoll_ctl(p_util.epoll_fd,EPOLL_DEL,all_event[i].data.fd);
				}*/

				//todo with :deal with data;
				add_task(p_util.child_process_pool,all_event[i].data.fd);
				/*printf("child recv client mes\n");
				char str[1024];
				bzero(str,1024);
				ssize_t readlen=read(all_event[i].data.fd,str,sizeof(str));*/
				/*if(readn>0)
					printf("%s\n",str);*/
			}
			else
			{
				continue;	
			}

		}



	}
}
void creaete_process()
{

	int i=0;
	p_util.m_index=-1;
	p_util.child_process_pool=NULL;
	for (i=0;i<CHILD_PROCESS_NUM;i++)
	{
		p_util.process_all[i].pid=-1;
		if(-1==socketpair(AF_UNIX,SOCK_STREAM,0,p_util.process_all[i].pipe_conv))
		{
			printf("create %d childpiepe error\n",i);	
		}
		p_util.process_all[i].pid=fork();
		p_util.close_process=true;
		if(p_util.process_all[i].pid>0)
		{

			close(p_util.process_all[i].pipe_conv[1]);
			continue;
		}
		else if(p_util.process_all[i].pid==0)
		{

			p_util.m_index=i;
			close(p_util.process_all[i].pipe_conv[0]);
			break;
		}
		else
		{
			printf("fork error");	
		}

	}
}
void add_sig(int sig)
{
	struct sigaction sa;
	memset(&sa,'\0',sizeof(sa));
	sa.sa_handler=sig_action;
	sa.sa_flags |=SA_RESTART;
	if(-1==sigfillset(&sa.sa_mask))
	{
		printf("sig fillset errolr\n");

	}
	if(-1==sigaction(sig,&sa,NULL))
	{
		printf("add sig failed\n");	
	}

}
void sig_action(int sig)
{
	int msg=sig;

	int err=	send(m_pipe_parent[1],(char*)&msg,1,0);
	if(err==-1)
	{
		printf("send sig error\n");	
	}
}
void setnonblocking(int fd)
{
	int old_char=fcntl(fd,F_GETFL);
	int new_char=old_char|O_NONBLOCK;
	fcntl(fd,F_SETFL,new_char);

}
