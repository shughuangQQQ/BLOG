#include"server_data_analy.h"
#include<sys/epoll.h>
#include<string.h>
void *analysedata(int client_fd,int m_epoll_fd)
{
	printf("begain to deal with client message by analyse\n");
	size_t read_buf;
	//NET_PACK* pack=(NET_PACK*)malloc(sizeof(NET_PACK));
	char* pack=(char *)malloc(PACK_SIZE);
	bzero(pack,PACK_SIZE);
	
	if((read_buf=read(client_fd,pack,PACK_SIZE))>0)
	{
		printf("%s\n",pack);	
		deal_with_data(pack,client_fd,m_epoll_fd);
		free(pack);
		pack=NULL;
	}
	else if(read_buf==0)
	{
	printf("client exit by accident\n");
	epoll_ctl(m_epoll_fd,EPOLL_CTL_DEL,client_fd,NULL);
	}

}
void deal_with_data(char* pack,int client_fd,int epollfd)
{
	
	cJSON*m_anly_json=cJSON_Parse(pack);
	//free(pack);
	cJSON*Packs_type=cJSON_GetObjectItem(m_anly_json,"PACK_TYPE");
	PAC_CODE_FEED feed_code;	
	if(!strcmp(Packs_type->valuestring,"login"))
	{
	
		feed_code=get_pass_access(m_anly_json);
		deal_feed_back(client_fd,feed_code,(char *)"login_feed");	
	}
	cJSON_Delete(m_anly_json);
}
PAC_CODE_FEED get_pass_access(cJSON*LOG_MES)
{
	cJSON*GetIDPAS=cJSON_GetObjectItem(LOG_MES,"User_Mes");
	cJSON*GetId=cJSON_GetObjectItem(GetIDPAS,"id");
	cJSON*GetPassward=cJSON_GetObjectItem(GetIDPAS,"passward");
	char user_path[256];
	bzero(user_path,256);
	FILE*fd;
	strcpy(user_path,"./SERVER_MESSAGE/");
	strcat(user_path,GetId->valuestring);
	if(!(fd=fopen(user_path,"r")))
	{
	return ID_UN_EXIST;	
	}
	char tempbuf[1024];
	bzero(tempbuf,1024);

	if(NULL==fgets(tempbuf,1024,fd))
	{
		fclose(fd);
	
	return SIGN_FIRST;	
	}
	char *savpoint=NULL;
	//char *savpoint1=NULL;
	//char *savpoint2=NULL;
	char *us=strtok_r(tempbuf,"=",&savpoint);
	//char *pasward=strtok(NULL,"=",&savpoint2);
	char *pasward=savpoint;
	printf("%s\n",pasward);
	if(!strcmp(pasward,GetPassward->valuestring))
	{
		fclose(fd);
	return PASSWARD_ERROR;	
	}
	

	fclose(fd);
	return LOG_SUCCESS;
}
void deal_feed_back(int client_fd,PAC_CODE_FEED feedback,char *pack_type,char* pack_body)
{
	cJSON* con_pack=cJSON_CreateObject();
	cJSON_AddStringToObject(con_pack,"PACK_TYPE",pack_type);
	if(pack_body==NULL)
		cJSON_AddStringToObject(con_pack,"PACK_BODY","NULL");
	else
		cJSON_AddStringToObject(con_pack,"PACK_BODY",pack_body);
	cJSON_AddNumberToObject(con_pack,"PACK_CODE_FEED",feedback);
	char *ch=cJSON_PrintUnformatted(con_pack);
	printf("send:%s to client\n",ch);
	ssize_t writewords;
	if((writewords=write(client_fd,ch,strlen(ch)))==-1)
	{
		printf("write to client error:%d\n",errno);	
	}
	cJSON_Delete(con_pack);
}

