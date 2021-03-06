#ifndef NET_PACK_H
#define NET_PACK_H


#define LISTEN 128
typedef enum {PACK_CHAR,PACK_ETC,PACK_HEAD_FILE}PACK_TYPE;
#define BODY_SIZE (4096)
#define TIAL_SIZE (1024)
#define PACK_BODY char
#define PACK_TAIL char
#define PACK_SIZE (sizeof(int)+sizeof(PACK_TYPE)+BODY_SIZE+TIAL_SIZE)



#define SERVER_PORT 8020
#define SERVER_IP "192.168.41.128"
//#define SERVER_IP "127.0.0.1"
typedef struct node2
{
    PACK_TYPE packtype;
    PACK_BODY packbody[BODY_SIZE];
    PACK_TAIL packtail[TIAL_SIZE];
    int file_size;

}NET_PACK;
typedef enum m_typ{ID_111,ID_UN_EXIST,ID_EXIST,SIGN_FIRST,SIGN_UP_SUCCESS,PASSWARD_ERROR,LOG_SUCCESS ,NO_FRIEND,FOUCS_SUCCESS,SET_HEAD_SUCCESS       }PAC_CODE_FEED;
#define MAX_FRIEND 100
#define MAX_NAME_LENGTH 15
#define CONFIG_PATH "./SERVER_MESSAGE/server_config"
typedef enum m_boll{FALSE,TRUE}BOOL;



#endif // NET_PACK_H
