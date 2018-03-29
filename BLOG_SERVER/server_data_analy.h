#ifndef DATADEAL
#define DATADEAL
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define TEMPBUF 4096
#define PACK_BODY_SIZE 1024
#define PACK_SIZE sizeof(NET_PACK)
typedef enum {PACK_CHAR,PACK_ETC}PACK_TYPE;
typedef struct node
{
	PACK_TYPE pack_type;
	char data[PACK_BODY_SIZE];
	int pack_tail;


}NET_PACK;


void deal_with_data(NET_PACK* pack)
{


}
void *analysedata(int client_fd)
{
	siez_t read_buf;
	NET_PACK* pack=(NET_PACK*)malloc(sizeof(NET_PACK));
	if((read_buf=read(client_fd,(void *)pack,PACK_SIZE))>0)
	{

		deal_with_data(pack);
	}


}

















#endif
