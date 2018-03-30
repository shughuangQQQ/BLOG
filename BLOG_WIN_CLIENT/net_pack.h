#ifndef NET_PACK_H
#define NET_PACK_H

#define SERVER_IP "192.168.1.1"
#define SERVER_PORT 8000
#include <qdebug.h>
typedef enum {PACK_LOG,PACK_CHAR,PACK_ETC}PACK_TYPE;

typedef enum {FALSE,TRUE}BOOL;
typedef struct node
{
    PACK_TYPE pack_type;
    char pack_body[4096];
    char pack_tail[1024];

}M_PACK;












#endif // NET_PACK_H
