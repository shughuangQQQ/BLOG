#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<errno.h>
#include<assert.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/stat.h>
typedef enum {PACK_CHAR,PACK_ETC}PACK_TYPE;
typedef enum {false true}bool;
#define SERVER_PORT 8000
#define MAX_LISTEN_NUM 100
#define PACK_BODY char 
#define BODY_SIZE 1024
#define TIAL_SIZE 1024
#define PACK_SIZE sizeof(conv_pack)
#define CHILD_PROCESS_NUM 5
#define nsjf 5
void run_parent_process();
void add_sig();
void sig_action(int sig);
void run_child_process();
void setnonblocking(int fd);
static int m_pipe_parent[2]=NULL;
typedef struct node
{
	pid_t pid;
	int pipe_conv[2];
}chilrd_process;
typedef struct node2
{
	PACK_TYPE packtype;
	PACK_BODY packbody[BODY_SIZE];
	PACK_TAIL packtial[TIAL_SIZE];

}conv_pack;
typedef struct node3
{

	chilrd_process*process_all[CHILD_PROCESS_NUM];
	bool close_process;
	int m_index;
	int epoll_fd;
	int servers_fd;

}process_utils;
void creaete_process();
void add_epoll_fd(int fd);
static process_utils p_util=NULL;
static int client_fd=0;



int main()
{


	int server_fd;
	p_util=(process_utils*)malloc(sizeof(process_utils));
	p_util.epoll_fd=create_epoll(MAX_LISTEN_NUM);
	p_util.close_process=true;
	struct sockaddr_in server_addr;
	int a=socket(server_fd,AF_INET,0);
	if(a==-1)
	{
		printf("scoket_create failed\n");	
	}
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htonl(SERVER_PORT);
	server_addr.sin_addr.s_addr=htons(INADDR_ANY);
	if(-1==bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr)))
	{
		printf("sock_bind failed\n");	
	}
	p_util.servers_fd=server_fd;
	if(-1==listen(server_fd,LISTEN))
	{
		printf("listen failed\n");	
	}

	creaete_process();
	if(p_util.m_index==-1)
	{
	run_parent_process();	
	}
	else
	{
	run_child_process();	
	}



	return 0;

}
void add_epoll_fd(int fd)
{

	struct epoll_event tmp_event;
	tmp_event.fd=fd;
	tmp_event.events=EPOLLIN;
	epoll_ctl(p_util.epoll_fd,EPOLL_CTL_ADD,fd,&tmp);
	setnonblocking(fd);
}
void run_parent_process()
{
	int errco=socketpair(AF_UNIX,SOCK_STREM,0,m_pipe_parent);
	add_epoll_fd(m_pipe_parent[0]);
	setnonblocking(m_pipe_parent[1]);
	add_sig(SIGCHLD,sig_action);
	add_sig(SIGINT,sig_action);
	add_sig(SIGPIPE,sig_action);
	add_sig(SIGTERM,sig_action);
	

	add_epoll_fd(p_util.servers_fd);
	int get_num,i;
	int chose_server=0;
	struct epoll_event all_event[MAX_LISTEN_NUM];
	int send_temp;
	int k;
	while(p_util.close_process)
	{
		get_num=epoll_wait(p_util.epoll_fd,epoll_events,MAX_LISTEN_NUM,-1);
		for(i=0;i<get_num;i++)
		{
			if(all_event[i].data.fd==p_util.servers_fd)
			{
				size_t sendbuf=send(p_util.chilrd_process[chose_server].pipe_conv[0],(char *)&send_temp,sizeof(send_temp),0);	
				if(sendbuf==-1)
				{
					printf("new lianjian send failed\n");	
				}
				chose_server=(chose_server+1)%CHILD_PROCESS_NUM;
			}
			else if(all_event[i].data.fd==m_pipe_parent[0])
			{
				char sig[1024];
				size_t temprecbuf=recv(m_pipe_parent[0],(char *)&sig,sizeof(sig),0);
				if(temprecbuf<=0)
					continue;
				for(k=0;k<temprecbuf;k++)
				{
					case SIGCHLD:
					{
						pid_t e_pid;
						while((pid=waitpid(-1,%stat,WHOHANG))>0)
						{
						for(i=1;i<CHILD_PROCESS_NUM;i++)
						{
						if(e_pid==p_util.chilrd_process[i].pid)
						{
								printf("close child process %d\n",pid);
								close(p_util.chilrd_process[i].pipe_conv[0]);
								p_util.chilrd_process[i].pid=-1;
						}

						}
						}
						p_util.close_process=false;


						
					break;
					}
					case SIGTERM:
					break;
					case SIGINT:
					{
					printf("close all server\n");
					for(i=0;i<CHILD_PROCESS_NUM;i++)
					{
					if(p_util.chilrd_process[i].pid!=-1)
					{
						kill(p_util.chilrd_process[i].pid,SIGTERM);	
					}
					}
					}
					break;
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
	int errco=socketpair(AF_UNIX,SOCK_STREM,0,m_pipe_parent);
	add_epoll_fd(m_pipe_parent[0]);
	setnonblocking(m_pipe_parent[1]);
	add_sig(SIGCHLD,sig_action);
	add_sig(SIGINT,sig_action);
	add_sig(SIGPIPE,sig_action);
	add_sig(SIGTERM,sig_action);

	struct sockaddr_in client_addr;
	int client_addr_size;
	while(p_util.close_process)
	{
		get_num=epoll_wait(p_util.epoll_fd,epoll_events,MAX_LISTEN_NUM,-1);
		for(i=0;i<get_num;i++)
		{
			if(all_event[i].data.fd==p_util.chilrd_process[m_index].pipe_conv[1])
			{
				size_t recbuf=recv(p_util.chilrd_process[m_index].pipe_conv[0],(char *)&recv_temp,sizeof(recv_temp),0);	
				if(recbuf==-1)
				{
					printf("new lianjian recv failed\n");	
				}
				client_fd=accept(p_util.servers_fd,(struct sockaddr*)&client_addr,&client_addr_size);
				if(-1==client_fd)
				{
				printf("recv client failed\n");	
				}
				add_epoll_fd(client_fd);

			}

			else if(all_event[i].data.fd==m_pipe_parent[0]&&(all_event[i].event & EPOLLIN)) 
			{
				char signals[1024];
				int k,m;
				int temp_recv=recv(m_pipe_parent[0],signals,sizeof(signals),0);
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
			else if(all_event[i].event & EPOLLIN)
			{
				//todo with :deal with data;
			
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
	p_util.process_all=(chilrd_process*)malloc(sizeof(chilrd_process)*CHILD_PROCESS_NUM);
	for (i=0;i<CHILD_PROCESS_NUM;i++)
	{
		p_util.process_all[i].pid=-1;
		if(-1==socketpair(AF_UNIX,SOCK_STREM,0,process_all[i].pipe_conv))
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
sigfillset(&sa.sa_mask);
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
