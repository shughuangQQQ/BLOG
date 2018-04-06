testver:blog_server.o pthread_task_deal.o server_data_analy.o
	gcc pthread_task_deal.o server_data_analy.o blog_server.o -o testver -lpthread
pthread_task_deal.o:pthread_task_deal.c
	gcc -c pthread_task_deal.c 
server_data_analy.o:server_data_analy.c
	gcc -c server_data_analy.c
blog_server.o:blog_server.c
	gcc -c blog_server.c
.PHONY:clean rebuild
clean:
	rm -rf *.o testver
rebuild:clean testver
	
