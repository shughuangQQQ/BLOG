#ifndef NET_INIT_PACK
#define NET_INIT_PACK
#define LISTEN 128
#define BODY_SIZE 1024
#define TIAL_SIZE 1024
#define PACK_BODY char
#define PACK_TAIL char
#define PACK_SIZE sizeof(NET_PACK)
typedef enum {PACK_CHAR,PACK_ETC}PACK_TYPE;
#include<errno.h>

typedef enum {false ,true}bool;
typedef struct node2
{
	PACK_TYPE packtype;
	PACK_BODY packbody[BODY_SIZE];
	PACK_TAIL packtail[TIAL_SIZE];
	
}NET_PACK;


#endif
