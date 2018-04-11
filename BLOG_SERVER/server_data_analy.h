#ifndef DATADEAL
#define DATADEAL
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define TEMPBUF 4096
#include"PAC_INET.h"
void deal_with_data(char *,int,int);
void *analysedata(int client_fd,int m_epoll_fd);
PAC_CODE_FEED get_pass_access(cJSON*);
void deal_feed_back(int,PAC_CODE_FEED,char *,char* =NULL);















#endif
