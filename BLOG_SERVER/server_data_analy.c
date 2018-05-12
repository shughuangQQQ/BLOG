#include"server_data_analy.h"
#include<sys/epoll.h>
#include<string.h>
#include<time.h>
#include<sys/types.h>
#include<sys/file.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
void m_strcpy(char *str1,char *str2)
{
	int i=0,j=0;
	for(i=0;i<strlen(str2);i++)
	{
		if(str2[i]=='\\')
		{	
			continue;
		}
		str1[j++]=str2[i];

	}
	printf("str1::%s\n",str1);
}
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
		cJSON*m_anly_json=cJSON_Parse(pack);
		if(m_anly_json)
		{	
			printf("%s\n",pack);	
			deal_with_data(m_anly_json,client_fd,m_epoll_fd);
			cJSON_Delete(m_anly_json);
		}
		else 
		{
			deal_with_file(pack,read_buf);
			printf("recv file\n");	
		}
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
void deal_with_file(char *pack,int read_length)
{
	NET_PACK* m_pack=(NET_PACK*)pack;
	if(m_pack->packtype==PACK_HEAD_FILE)
	{
	Save_Head(m_pack);	
	}
}
void deal_with_data(cJSON* m_anly_json,int client_fd,int epollfd)
{

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
	else if(!strcmp(Packs_type->valuestring,"SIGN_UP"))
	{

		feed_code=sign_up_acess(m_anly_json);
		deal_feed_back(client_fd,feed_code,(char *)"sign_up_feed");	
	}
	else if(!strcmp(Packs_type->valuestring,"FOCUS"))
	{

		feed_code=foucus_on_friend(m_anly_json);
		//deal_feed_back(client_fd,feed_code,(char *)"sign_up_feed");	
	}
	else if(!strcmp(Packs_type->valuestring,"push_static"))
	{

		feed_code=push_person_mes(m_anly_json);
		//deal_feed_back(client_fd,feed_code,(char *)"push_static_feed");	
	}/*
		else if(!strcmp(Packs_type->valuestring,"get_friend_static"))
		{

		feed_code=push_person_mes(m_anly_json);
		deal_feed_back(client_fd,feed_code,(char *)"push_static_feed");	
		}*/
	else if(!strcmp(Packs_type->valuestring,"MeMes"))
	{
		cJSON* body_pack=cJSON_CreateObject();
		feed_code=Get_Myself_Mes(m_anly_json,body_pack);
		deal_feed_back(client_fd,feed_code,(char *)"MeMes_FEED",NULL,body_pack);	
		//cJSON_Delete(body_pack);


	}

}
PAC_CODE_FEED Save_Head(NET_PACK*m_data)
{

	

	FILE*fd;
	char user_path[256];
	bzero(user_path,256);

	strcpy(user_path,"./SERVER_MESSAGE/");
	strcat(user_path,m_data->packtail);
	strcat(user_path,".jpg");
	if(0==access(user_path,F_OK))
	{
		remove(user_path);	
	}
	if(!(fd=fopen(user_path,"a")))
	{
		printf("open error %d\n",errno);
		return ID_UN_EXIST;
	}
	/*if(EOF==fputs(GetPix->valuestring,fd))
	{
		printf("fputs error %d\n",errno);	
	}*/
	write(fd->_fileno,m_data->packbody,m_data->file_size);

	fclose(fd);
	return SET_HEAD_SUCCESS;
}
PAC_CODE_FEED Get_Myself_Mes(cJSON* m_Json,cJSON *body_pack)
{

	cJSON*GetId=cJSON_GetObjectItem(m_Json,"id");
	char user_path[256];
	bzero(user_path,256);
	char tempbuf[1024];
	bzero(tempbuf,1024);

	FILE*fd;
	char *get_mes[4]={(char*)"fans_num",(char *)"friend_num",(char *)"blogitem_num",(char *)"Brief"};
	int fannum=0;
	int friendnum=0;
	int blogitem_num=0;
	char Brief[256];
	bzero(Brief,256);
	strcpy(user_path,"./SERVER_MESSAGE/");
	strcat(user_path,GetId->valuestring);
	if(!(fd=fopen(user_path,"r+")))
	{
		printf("fopen error%d\n",errno);
		return ID_UN_EXIST;	
	}
	if(0!=(flock(fd->_fileno,LOCK_EX)))
		printf("file already lock,waite for unlock\n");
	int i=0,j=0;
	while(fgets(tempbuf,1024,fd))
	{

		char *savpoint=NULL;
		char *us=strtok_r(tempbuf,"=",&savpoint);
		for(i=j;i<4;i++)
		{
			if(strncmp(us,get_mes[i],strlen(get_mes[i]))==0)
			{
				printf("find::%s\n",get_mes[i]);
				j++;
				if(i==0)			
					fannum=atoi(savpoint);
				else if(i==1)
					friendnum=atoi(savpoint);
				else if(i==2)
					blogitem_num=atoi(savpoint);
				else if(i==3)
					strcpy(Brief,savpoint);
			}
		}
	}


	fclose(fd);
	flock(fd->_fileno,LOCK_UN);

	cJSON_AddNumberToObject(body_pack,"fans_num",fannum);
	cJSON_AddNumberToObject(body_pack,"friend_num",friendnum);
	cJSON_AddNumberToObject(body_pack,"blogitem_num",blogitem_num);
	cJSON_AddStringToObject(body_pack,"Brief",Brief);
	//char *ch=cJSON_PrintUnformatted(body_pack);
	//m_strcpy(m_mes,ch);
	//cJSON_Delete(body_pack);
	return FOUCS_SUCCESS;
}
PAC_CODE_FEED push_person_mes(cJSON*m_Json)//发布个人动态
{

	cJSON*GetId=cJSON_GetObjectItem(m_Json,"id");
	cJSON*GetPushMes=cJSON_GetObjectItem(m_Json,"push_mes");
	char user_path[256];
	bzero(user_path,256);
	strcpy(user_path,"./SERVER_MESSAGE/");
	strcat(user_path,GetId->valuestring);
	//strcat(user_path,".mes");
	FILE*fd;
	if(-1==access(user_path,F_OK))
		return ID_UN_EXIST;
	if(!(fd=fopen(user_path,"r+")))
	{
		printf("fopen r error%d\n",errno);

	}
	if(0!=(flock(fd->_fileno,LOCK_EX)))
		printf("file already lock,waite for unlock\n");
	char tempbuf[1024];
	bzero(tempbuf,1024);

	int blog_num_line=0;
	int num=0;
	int i;
	char temnum[10];
	bzero(temnum,10);
	while(fgets(tempbuf,1024,fd))
	{
		printf("%s\n",tempbuf);	
		blog_num_line++;
		char *savpoint=NULL;
		char *us=strtok_r(tempbuf,"=",&savpoint);

		if(0==strncmp(us,"blogitem_num",strlen("blogitem_num")))
		{
			printf("find\n");
			for(i=0;savpoint[i]!='\n';i++)
			{
				temnum[i]=savpoint[i];	
			}
			num=atoi(temnum);
			num++;
			bzero(temnum,10);
			sprintf(temnum,"%d",num);


			if(-1==fseek(fd,0,SEEK_SET))
			{
				printf("seek error %d\n",errno);

				return ID_UN_EXIST;
			}
			break;
		}

	}
	if(num==0)
		return ID_UN_EXIST;
	printf("blog_num_line %d\n",blog_num_line);
	remove_line(fd,blog_num_line,user_path);
	if(-1==fseek(fd,0,SEEK_END))
	{
		printf("seek error %d\n",errno);
		return ID_UN_EXIST;
	}
	char write_buf[1024];
	bzero(write_buf,1024);
	strcpy(write_buf,"blogitem_num=");
	strcat(write_buf,temnum);
	strcat(write_buf,"\n");
	printf("write %s\n",write_buf);
	if(EOF==fputs(write_buf,fd))
	{
		printf("fputs error %d\n",errno);	
	}
	bzero(write_buf,1024);
	strcpy(write_buf,"blog=");
	strcat(write_buf,GetPushMes->valuestring);
	strcat(write_buf,"\n");
	printf("write %s\n",write_buf);
	if(EOF==fputs(write_buf,fd))
	{
		printf("fputs error %d\n",errno);	
	}

	flock(fd->_fileno,LOCK_UN);
	fclose(fd);
}
/*
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
	if(0!=(flock(fd->_fileno,LOCK_EX)))
		printf("file already lock,waite for unlock\n");
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
	flock(fd->_fileno,LOCK_UN);
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
	if(-1==access(user_path,F_OK))
		return ID_UN_EXIST;
	char tempbuf[1024];
	bzero(tempbuf,1024);
	if(!(fd=fopen(user_path,"r")))
	{
		printf("log error id doesent exist %s error code:%d\n",user_path,errno);
		return ID_UN_EXIST;	
	}
	if(0!=flock(fd->_fileno,LOCK_EX))
	{
		printf("lock errno%d\n",errno);
	}

	if(NULL==fgets(tempbuf,1024,fd))
	{
		fclose(fd);

		return SIGN_FIRST;	
	}
	fclose(fd);
	flock(fd->_fileno,LOCK_UN);

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
		return PASSWARD_ERROR;	
	}


	return LOG_SUCCESS;
}
void deal_feed_back(int client_fd,PAC_CODE_FEED feedback,char *pack_type,char* pack_body,cJSON*json_body)
{
	cJSON* con_pack=cJSON_CreateObject();
	cJSON_AddStringToObject(con_pack,"PACK_TYPE",pack_type);
	if(pack_body==NULL&&json_body==NULL)
		cJSON_AddStringToObject(con_pack,"PACK_BODY","NULL");
	else if(json_body!=NULL)
		cJSON_AddItemToObject(con_pack,"PACK_BODY",json_body);
	else 
		cJSON_AddStringToObject(con_pack,"PACK_BODY",pack_body);

	cJSON_AddNumberToObject(con_pack,"PACK_CODE_FEED",feedback);
	char *ch=cJSON_PrintUnformatted(con_pack);
	//char send_ch[4096];
	//bzero(send_ch,4096);

	//m_strcpy(send_ch,ch);
	printf("send:%s to client\n",/*send_*/ch);
	ssize_t writewords;
	if((writewords=write(client_fd,/*send_*/ch,strlen(/*send_*/ch)))==-1)
	{
		printf("write to client error:%d\n",errno);	
	}
	cJSON_Delete(con_pack);
}

PAC_CODE_FEED sign_up_acess(cJSON* SIGN_MES)
{
	cJSON*GetId=cJSON_GetObjectItem(SIGN_MES,"id");

	cJSON*GetPassward=cJSON_GetObjectItem(SIGN_MES,"passward");


	char user_path[256];
	bzero(user_path,256);
	FILE*fd;
	strcpy(user_path,"./SERVER_MESSAGE/");
	strcat(user_path,GetId->valuestring);
	if(0==access(user_path,F_OK))
		return ID_EXIST;
	if(!(fd=fopen(user_path,"a")))
	{

		printf("open error %d\n",errno);
		return ID_EXIST;
	}
	if(0!=(flock(fd->_fileno,LOCK_EX)))
		printf("file already lock,waite for unlock\n");
	char *userid=GetId->valuestring;
	char *pasward=GetPassward->valuestring;
	char writetemp_buf[1024];
	bzero(writetemp_buf,1024);
	strcpy(writetemp_buf,userid);
	strcat(writetemp_buf,"=");
	strcat(writetemp_buf,pasward);
	strcat(writetemp_buf,"\n");

	if(EOF==fputs(writetemp_buf,fd))
	{
		printf("error on write new sign_up file %d\n",errno);
	}
	if(EOF==fputs("friend=shugh\n",fd))
	{
		printf("error on write new sign_up file %d\n",errno);
	}
	if(EOF==fputs("fans=shugh\n",fd))
	{
		printf("error on write new sign_up file %d\n",errno);
	}
	if(EOF==fputs("fans_num=shugh\n",fd))
	{
		printf("error on write new sign_up file %d\n",errno);
	}
	if(EOF==fputs("Brief=nothing\n",fd))
	{
		printf("error on write new sign_up file %d\n",errno);
	}
	if(EOF==fputs("blogitem_num=0\n",fd))
	{
		printf("error on write new sign_up file %d\n",errno);
	}

	fclose(fd);
	flock(fd->_fileno,LOCK_UN);

	return SIGN_UP_SUCCESS;


}
