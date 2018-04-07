#include"server_data_analy.h"
#include<errno.h>
#include<sys/epoll.h>

void *analysedata(int client_fd,int m_epoll_fd)
{
	printf("begain to deal with client message by analyse\n");
	size_t read_buf;
	NET_PACK* pack=(NET_PACK*)malloc(sizeof(NET_PACK));

	
	if((read_buf=read(client_fd,(void *)pack,PACK_SIZE))>0)
	{

		deal_with_data(pack);
	}
	else if(read_buf==0)
	{
	printf("client exit by accident\n");
	epoll_ctl(m_epoll_fd,EPOLL_CTL_DEL,client_fd,NULL);
	}

}
void deal_with_data(NET_PACK* pack)
{
	printf("okokokok\n");

}


