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
void deal_feed_back(int,PAC_CODE_FEED,char *,char* =NULL,cJSON * =NULL);
PAC_CODE_FEED sign_up_acess(cJSON*);
PAC_CODE_FEED foucus_on_friend(cJSON*);
void remove_line(FILE*file,int line_num);
PAC_CODE_FEED Get_Find_User(cJSON *);
PAC_CODE_FEED push_person_mes(cJSON*);
PAC_CODE_FEED Get_Myself_Mes(cJSON*,cJSON*);
char * getdata();
void m_strcpy(char *,char*);
PAC_CODE_FEED Save_Head(cJSON*);







#endif
