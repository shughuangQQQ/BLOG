#include"server_data_analy.h"
#include<sys/epoll.h>
#include<string.h>
#include<time.h>
void remove_line(FILE*fp,int linenum,char *path)
{



	char buf[4096];



	int total_line = 0; // 记录文件总行数
	int size = 0;       // 记录文件总大小

	while (fgets(buf, sizeof(buf), fp)) {  
		size += strlen(buf);
		total_line++;
	}

	// 如果要删除文件的行数大于文件总行数，退出程序
	if (linenum > total_line) {
		printf("%d is greater than total_line!\n", linenum);
		return ;
	}


	int s = 0;      // 记录要删除行大小
	int t = 0;      // 记录每一行大小
	int i = 0;      // 记录当前行数
	fseek(fp, 0L, SEEK_SET);    // 将文件指针移到文件头

	// 将要删除行后的每一行前移一行
	while (fgets(buf, sizeof(buf), fp)) {  

		i++;
		t = strlen(buf);

		if (i == linenum) {
			s = t;
		}

		if (i > linenum) {
			fseek(fp, -(s+t), SEEK_CUR);
			fputs(buf, fp);
			fseek(fp, s, SEEK_CUR);
		}
	} 

	truncate(path, size-s);  // 截断文件


	return ;
}
char *getdata()
{
	char nowtime[40]; 
	struct tm *ltime;
	time_t timep;
	time(&timep);
	ltime = localtime(&timep);
	strftime(nowtime, 20, "%Y-%m-%d %H:%M:%S", ltime);  
	char *time_ret=(char *)malloc(40);
	strcpy(time_ret,nowtime);
	return time_ret;
}
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
PAC_CODE_FEED Get_Find_User(cJSON* m_object)
{
	cJSON*FINDMES=cJSON_GetObjectItem(m_object,"FIND_MES");
	cJSON*FIND_ID=cJSON_GetObjectItem(FINDMES,"find");
	printf("%s\n",FIND_ID->valuestring);
	char path[1024];
	bzero(path,1024);
	strcpy(path,"./SERVER_MESSAGE/");
	strcat(path,FIND_ID->valuestring);
	if(-1==access(path,F_OK))
		return ID_UN_EXIST;
	return ID_EXIST;

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
	else if(!strcmp(Packs_type->valuestring,"FIND"))
	{
		
		feed_code=Get_Find_User(m_anly_json); 
		deal_feed_back(client_fd,feed_code,(char *)"FIND_FEED");	
		
	}
	/*else if(!strcmp(Packs_type->valuestring,"sign_up"))
	{

		feed_code=sign_up_acess(m_anly_json);
		deal_feed_back(client_fd,feed_code,(char *)"sign_up_feed");	
	}*/
	else if(!strcmp(Packs_type->valuestring,"FOCUS"))
	{

		feed_code=foucus_on_friend(m_anly_json);
		//deal_feed_back(client_fd,feed_code,(char *)"sign_up_feed");	
	}/*
	else if(!strcmp(Packs_type->valuestring,"push_person_static"))
	{

		feed_code=push_person_mes(m_anly_json);
		deal_feed_back(client_fd,feed_code,(char *)"push_static_feed");	
	}
	else if(!strcmp(Packs_type->valuestring,"get_friend_static"))
	{

		feed_code=push_person_mes(m_anly_json);
		deal_feed_back(client_fd,feed_code,(char *)"push_static_feed");	
	}*/

	cJSON_Delete(m_anly_json);
}
/*PAC_CODE_FEED push_person_mes(cJSON*m_Json)//发布个人动态
{

	cJSON*GetIDPAS=cJSON_GetObjectItem(m_Json,"User_Mes");
	cJSON*GetId=cJSON_GetObjectItem(GetIDPAS,"id");
	cJSON*GetPassward=cJSON_GetObjectItem(GetIDPAS,"passward");
	cJSON*AddPersonMes=cJSON_GetObjectItem(m_Json,"Add_Mes_Static")
		char user_path[256];
	bzero(user_path,256);
	strcpy(user_path,"./SERVER_MESSAGE/");
	strcat(user_path,GetId->valuestring);
	strcat(user_path,".mes");
	FILE*fd,*fd2;
	if(!(fd=fopen(user_path,"a")))
	{
		printf("fopen a error%d\n",errno);
		return ID_UN_EXIST;	
	}
	if(!(fd2=fopen(CONFIG_PATH,"r+")))
	{
		printf("fopen r error%d\n",errno);

	}
	char read_list[1024];
	if((EOF==fgets(read_list,1024,fd2)))
		printf("error on read_list file %d\n",errno);
	char *save_1=NULL;
	char*first=strtok_r(read_list,"=",&save_1);
	int listnum=atoi(save_1);
	listnum++;
	char str1[100];
	char tempbuf[4096];
	bzero(tempbuf);
	itoa(listnum,tempbuf,10);
	strcpy(tempbuf,first);
	strcat(tempbuf,"=");
	strcat(tempbuf,tempbuf);

	if(!fseek(fd2,0,SEEK_SET))
	{
		printf("seek error %d\n",errno);
		return ID_UN_EXIST;
	}
	if((EOF==fputs(tempbuf,fd2)))
		printf("error on write new sign_up file %d\n",errno);
	fclose(fd2);
	char *m_data=getdata();

	char write1[1024];

	//strcpy(write1,itoa());
	if((EOF==fputs(tempbuf,fd)))
		printf("error on write new sign_up file %d\n",errno);
	if((EOF==fputs(m_data,fd)))
		printf("error on write new sign_up file %d\n",errno);
	if(EOF==fputs(AddPersonMes->valuestring,fd))
		printf("error on write new sign_up file %d\n",errno);
	fclose(fd);
	free(m_data);
}

PAC_CODE_FEED get_friend_static(cJSON*m_Json)
{
	cJSON*GetIDPAS=cJSON_GetObjectItem(m_Json,"User_Mes");
	cJSON*GetId=cJSON_GetObjectItem(GetIDPAS,"id");
	cJSON*GetPassward=cJSON_GetObjectItem(GetIDPAS,"passward");
	char user_path[256];
	bzero(user_path,256);
	FILE*fd;
	strcpy(user_path,"./SERVER_MESSAGE/");
	strcat(user_path,GetId->valuestring);
	if(!(fd=fopen(user_path,"r")))
	{
		printf("fopen error%d\n",errno);
		return ID_UN_EXIST;	
	}//只返回三日内的动态
	char *friend_list[MAX_FRIEND];
	int i=0;
	for (i=0;i<MAX_FRIEND;i++)
	{
		friend_list[i]=NULL;

	}
	char tempbuf[1024];
	bzero(tempbuf,1024);
	char *friendlist=NULL;
	while(fgets(tempbuf,1024,fd))
	{

		char *savpoint=NULL;
		char *us=strtok_r(tempbuf,"=",&savpoint);

		if(strcmp(us,"friend")==0)
		{
			friendlist=strtok_r(NULL,"=",&savpoint);
			fclose(fd);
			break;
		}

	}
	i=0;
	char *savpoint2=NULL;
	char *getname_list=NULL;
	while((getname_list=strtok_r(friendlist,",",&savpoint2)))
	{
		friend_list[i]=(char *)malloc(MAX_NAME_LENGTH);
		bzero(friend_list[i],MAX_NAME_LENGTH);
		strcpy(friend_list[i++],getname_list);
	}
	int h;
	char *datastr=getdata();
	char tempbuf2[4096];
	for(h=0;h<i;h--)
	{
		bzero(user_path,256);

		FILE*fd_friend;
		strcpy(user_path,"./SERVER_MESSAGE/");
		strcat(user_path,friend_list[h]);
		strcat(user_path,".mes");
		if(!(fd_friend=fopen(user_path,"r")))
		{
			printf("fopen error%d\n",errno);

		}

		while(fgets(tempbuf2,4096,fd_friend))
		{



		}



		fclose(fd_friend);

	}

	free(datastr);
	datastr=NULL;



}
*/
PAC_CODE_FEED foucus_on_friend(cJSON*m_Json)
{
	cJSON*GetId=cJSON_GetObjectItem(m_Json,"id");
	cJSON*GetAddFriend=cJSON_GetObjectItem(m_Json,"focus_id");
	int m_line=0;
	char *friend_name=GetAddFriend->valuestring;
	char user_path[256];
	bzero(user_path,256);
	FILE*fd;
	strcpy(user_path,"./SERVER_MESSAGE/");
	strcat(user_path,GetId->valuestring);
	if(!(fd=fopen(user_path,"r+")))
	{
		printf("fopen error%d\n",errno);
		return ID_UN_EXIST;	
	}
	char tempbuf[1024];
	bzero(tempbuf,1024);
	char writebuf[1024];
	bzero(writebuf,1024);
	while(fgets(tempbuf,1024,fd))
	{

		m_line++;
		strcpy(writebuf,tempbuf);
		char *savpoint=NULL;
		char *us=strtok_r(tempbuf,"=",&savpoint);

		if(strcmp(us,"friend")==0)
		{

			if(-1==fseek(fd,0,SEEK_SET))
			{
				printf("seek error %d\n",errno);
				return ID_UN_EXIST;
			}
			break;
		}
		bzero(tempbuf,1024);
	}
	remove_line(fd,m_line,user_path);

	writebuf[strlen(writebuf)-1]=',';
	strcat(writebuf,friend_name);

	if(-1==fseek(fd,0,SEEK_END))
	{
		printf("seek error %d\n",errno);
	}


	if(EOF==fputs(writebuf,fd))
	{
		printf("error on write new sign_up file %d\n",errno);
	}
	fclose(fd);
	return FOUCS_SUCCESS;

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
		printf("log error id doesent exist %s error code:%d\n",user_path,errno);
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
	//printf("server passward%s\n",pasward);
	//printf("user passward%s\n",GetPassward->valuestring);
	if(0!=strncmp(pasward,GetPassward->valuestring,strlen(pasward)-1))
	{
		printf("passward error\n%s\n%s\n",pasward,GetPassward->valuestring);
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

/*PAC_CODE_FEED sign_up_acess(cJSON* SIGN_MES)
{
	cJSON*GetIDPAS=cJSON_GetObjectItem(SIGN_MES,"User_Mes");
	cJSON*GetId=cJSON_GetObjectItem(GetIDPAS,"id");
	cJSON*GetPassward=cJSON_GetObjectItem(GetIDPAS,"passward");


	char user_path[256];
	bzero(user_path,256);
	FILE*fd;
	strcpy(user_path,"./SERVER_MESSAGE/");
	strcat(user_path,GetId->valuestring);
	if((fd=fopen(user_path,"r")))
	{
		fclose(fd);
		return ID_EXIST;	
	}
	if(!(fd=fopen(user_path,"a")))
	{

		printf("open error %d\n",errno);
	}
	char *userid=GetId->valuestring;
	char *pasward=GetPassward->valuestring;
	char writetemp_buf[1024];
	bzero(writetemp_buf,1024);
	strcpy(writetemp_buf,userid);
	strcpy(writetemp_buf,"=");
	strcpy(writetemp_buf,passward);

	if(EOF==fputs(writetemp_buf,fd))
	{
		printf("error on write new sign_up file %d\n",errno);
	}
	if(EOF==fputs("friend=shugh",fd))
	{
		printf("error on write new sign_up file %d\n",errno);
	}
	fclose(fd);
	return SIGN_UP_SUCCESS;


}*/
