#ifndef NET_PACK_H
#define NET_PACK_H

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8020
#include <qdebug.h>
typedef enum {PACK_LOG,PACK_CHAR,PACK_ETC}PACK_TYPE;
typedef enum m_typ{ID_111,
                   ID_UN_EXIST,
                   ID_EXIST,
                   SIGN_FIRST,
                   SIGN_UP_SUCCESS,
                   PASSWARD_ERROR,
                   LOG_SUCCESS ,
                   NO_FRIEND,
                   FOUCS_SUCCESS}PAC_CODE_FEED;
typedef enum {FALSE,TRUE}BOOL;
typedef struct node
{
    PACK_TYPE pack_type;
    char pack_body[4096];
    char pack_tail[1024];

}M_PACK;












#endif // NET_PACK_H
