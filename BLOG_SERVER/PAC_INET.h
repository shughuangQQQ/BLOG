#ifndef NET_INIT_PACK
#define NET_INIT_PACK
#define LISTEN 128
#define BODY_SIZE 1024
#define TIAL_SIZE 1024
#define PACK_BODY char
#define PACK_TAIL char
#define PACK_SIZE 4096
typedef enum {PACK_CHAR,PACK_ETC}PACK_TYPE;
#include<errno.h>
#include"cJSON/cJSON.h"
//typedef enum {false ,true}bool;
#define SERVER_PORT 8020
#define SERVER_IP_1 "192.168.41.128"
typedef struct node2
{
	PACK_TYPE packtype;
	PACK_BODY packbody[BODY_SIZE];
	PACK_TAIL packtail[TIAL_SIZE];
	
}NET_PACK;
typedef enum m_typ{ID_UN_EXIST,ID_EXIST,SIGN_FIRST,SIGN_UP_SUCCESS,PASSWARD_ERROR,LOG_SUCCESS ,NO_FRIEND,FOUCS_SUCCESS       }PAC_CODE_FEED;


#endif
