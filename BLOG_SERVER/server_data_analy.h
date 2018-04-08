#ifndef DATADEAL
#define DATADEAL
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define TEMPBUF 4096
#include"PAC_INET.h"
void deal_with_data(char *);
void *analysedata(int client_fd,int m_epoll_fd);

















#endif
