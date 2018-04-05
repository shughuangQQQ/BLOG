#ifndef DATADEAL
#define DATADEAL
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define TEMPBUF 4096
#include"PAC_INET.h"
void deal_with_data(NET_PACK* pack);
void *analysedata(int client_fd);



void *analysedata(int client_fd)
{
	size_t read_buf;
	NET_PACK* pack=(NET_PACK*)malloc(sizeof(NET_PACK));

	
	if((read_buf=read(client_fd,(void *)pack,PACK_SIZE))>0)
	{

		deal_with_data(pack);
	}


}
void deal_with_data(NET_PACK* pack)
{


}

















#endif
