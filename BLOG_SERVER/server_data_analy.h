#ifndef DATADEAL
#define DATADEAL
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define PACK_BODY_SIZE 1024
typedef enum {PACK_CHAR,PACK_ETC}PACK_TYPE;
typedef struct node
{
	PACK_TYPE pack_type;
	char data[PACK_BODY_SIZE];
	int pack_tail;


}NET_PACK;




















#endif
