#include"server_data_analy.h"
void *analysedata(int client_fd)
{
	printf("begain to deal with client message by analyse\n");
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


