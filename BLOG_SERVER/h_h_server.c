
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <assert.h>  
#include <stdio.h>  
#include <unistd.h>  
#include <errno.h>  
#include <string.h>  
#include <fcntl.h>  
#include <stdlib.h>  
#include <sys/epoll.h>  
#include <signal.h>  
#include <sys/wait.h>  
#include <sys/stat.h>  
  
/*描述一个子进程的类，m_pid是目标子进程的pid，m_pipefd是父进程和子进程通信用的管道*/  
class process  
{  
public:  
    process() : m_pid( -1 ){}  
  
public:  
    pid_t m_pid;//子进程号初始化为-1  
    int m_pipefd[2];//与父进程通信用的管道，主要是父进程通知子进程接受连接  
};  
  
/*进程池类，将它定义为模板类是为了代码的复用。其模板参数是处理逻辑任务的类*/  

class processpool  
{  
private:  
    /*将构造函数定义为私有的，因此我们只能通过后面的create静态函数来创建processpool类实例*/  
    processpool( int listenfd, int process_number = 8 );//构造函数申明为私有函数，构造函数由create完成  
public:  
    /*单体模式，以保证程序最多创建一个processpool实例，这是程序正确处理信号的必要条件*/  
    static processpool * create( int listenfd, int process_number = 8 )//线程池为单件模式  
    {  
        if( !m_instance )  
        {  
            m_instance = new processpool ( listenfd, process_number );//listenfd是服务端监听端口，process_number池中线程数  
        }  
        return m_instance;//返回线程池实例  
    }  
    ~processpool()  
    {  
        delete [] m_sub_process;//删除子进程  
    }  
    void run();//启动线程池  
  
private:  
    void setup_sig_pipe();//信号管道用于统一事件源  
    void run_parent();//专用调度进程(这里称为父进程)  
    void run_child();//工作进程(这里称为子进程)  
  
private:
    static const int MAX_PROCESS_NUMBER = 16;//进程池允许的最大子进程数量  
    static const int USER_PER_PROCESS = 65536;//每个工作进程(子进程)处理的最大连接数（客户数量）  
    static const int MAX_EVENT_NUMBER = 10000;//epoll最多能处理的事件数  
    int m_process_number;//进程池中进程总数  
    int m_idx;//每个子进程在池中的编号，从0开始  
    int m_epollfd;//epoll内核事件表标识符  
    int m_listenfd;//服务端监听socket  
    int m_stop;//子进程通过该标识来决定是否停止运行  
    process* m_sub_process;//保存所有子进程的描述信息，子进程数组，通过m_idx索引到具体子进程类  
    static processpool * m_instance;//申明进程池静态实例  
};  
  

processpool * processpool ::m_instance = NULL;//类静态变量定义  
  
/*用于处理信号的管道，以实现统一事件源*/  
static int sig_pipefd[2];//信号管道  
  
static int setnonblocking( int fd )//设置描述符fd为非阻塞  
{  
    int old_option = fcntl( fd, F_GETFL );  
    int new_option = old_option | O_NONBLOCK;  
    fcntl( fd, F_SETFL, new_option );  
    return old_option;  
}  
  
static void addfd( int epollfd, int fd )//将描述符fd添加到事件表epollfd注册可读事件  
{  
    epoll_event event;  
    event.data.fd = fd;  
    event.events = EPOLLIN | EPOLLET;  
    epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );  
    setnonblocking( fd );//设置非阻塞  
}  
  
/*从epollfd标识的epoll内核事件表中删除fd上的所有注册事件*/  
static void removefd( int epollfd, int fd )  
{  
    epoll_ctl( epollfd, EPOLL_CTL_DEL, fd, 0 );  
    close( fd );  
}  
  
static void sig_handler( int sig )//信号处理函数将信号值发送给进程(统一事件源)  
{  
    int save_errno = errno;  
    int msg = sig;  
    send( sig_pipefd[1], ( char* )&msg, 1, 0 );//发送信号值给进程  
    errno = save_errno;  
}  
  
static void addsig( int sig, void( handler )(int), bool restart = true )//信号安装函数  
{  
    struct sigaction sa;//结构体描述信号处理的细节  
    memset( &sa, '\0', sizeof( sa ) );  
    sa.sa_handler = handler; //指定信号处理函数  
    if( restart )  
    {  
        sa.sa_flags |= SA_RESTART; //设置程序收到信号时的行为，该行为添加重新调用被该信号终止的系统调用  
    }  
    sigfillset( &sa.sa_mask ); //在信号集中设置该信号sa_mask为信号掩码，或操作为添加信号  
    assert( sigaction( sig, &sa, NULL ) != -1 ); //判断信号处理函数是否成功运行  
}  
  
/*进程池构造函数，参数listenfd是监听socket，它必须在创建进程池之前被创建，否则子进程无法直接引用它。参数process_number指定进程池中子进程的数量*/  

processpool ::processpool( int listenfd, int process_number )   
    : m_listenfd( listenfd ), m_process_number( process_number ), m_idx( -1 ), m_stop( false )  
{  
    assert( ( process_number > 0 ) && ( process_number <= MAX_PROCESS_NUMBER ) );  
  
    m_sub_process = new process[ process_number ];//进程数组通过m_idx为下标索引到具体子进程  
    assert( m_sub_process );  
  
   /*创建process_number个子进程，并建立它们与父进程之间的通道*/  
    for( int i = 0; i < process_number; ++i )  
    {  
        int ret = socketpair( PF_UNIX, SOCK_STREAM, 0, m_sub_process[i].m_pipefd );//主要用于父进程有新的客户连接需要子进程处理，全双工，两端都可以读写  
        assert( ret == 0 );  
  
        m_sub_process[i].m_pid = fork();//创建子进程  
        assert( m_sub_process[i].m_pid >= 0 );  
        if( m_sub_process[i].m_pid > 0 )//父进程  
        {  
            close( m_sub_process[i].m_pipefd[1] );  
            continue;//继续创建子进程  
        }  
        else  
        {  
            close( m_sub_process[i].m_pipefd[0] );  
            m_idx = i;//子进程在池中的标号为m_idx，主要通过子进程数组快速索引到子进程类  
            break;//子进程不用创建进程所以跳出  
        }  
    }  
}  
  
/*统一事件源*/  

void processpool ::setup_sig_pipe()  
{  
    /*创建epoll事件监听表和信号管道*/  
    m_epollfd = epoll_create( 5 );  
    assert( m_epollfd != -1 );  
  
    int ret = socketpair( PF_UNIX, SOCK_STREAM, 0, sig_pipefd );  
    assert( ret != -1 );  
  
    setnonblocking( sig_pipefd[1] );  
    addfd( m_epollfd, sig_pipefd[0] );//将信号管道添加到事件表用以统一事件源  
  
    /*设置信号处理函数*/  
    addsig( SIGCHLD, sig_handler );//添加信号  
    addsig( SIGTERM, sig_handler );  
    addsig( SIGINT, sig_handler );  
    addsig( SIGPIPE, SIG_IGN );  
}  
  
/*父进程中m_idx值为-1，子进程中m_idx值大于等于0，根据此判断接下来要运行的是父进程代码还是子进程代码*/  

void processpool ::run()//进程池启动函数  
{  
    if( m_idx != -1 )//m_idx是子进程，在进程池中的编号不为-1表示是子进程  
    {  
        run_child();//启动子进程  
        return;  
    }  
    run_parent();//启动父进程  
}  
void processpool::run_child()//子进程逻辑  
{
    setup_sig_pipe();//启动信号管道  
    /*每个子进程都通过其在进程池的序号值m_idx找到与父进程通信的管道*/  
    int pipefd = m_sub_process[m_idx].m_pipefd[ 1 ];  
    /*子进程需要监听管道文件描述符pipefd，因为父进程将通过它来通知子进程accept新连接*/  
    addfd( m_epollfd, pipefd );  
  
    epoll_event events[ MAX_EVENT_NUMBER ];  
    T* users = new T [ USER_PER_PROCESS ];//模板参数T是任务类型，该子进程能处理的连接数组  
    assert( users );  
    int number = 0;  
    int ret = -1;  
  
    while( ! m_stop )  
    {  
        number = epoll_wait( m_epollfd, events, MAX_EVENT_NUMBER, -1 );//监听事件  
        if ( ( number < 0 ) && ( errno != EINTR ) )  
        {  
            printf( "epoll failure\n" );  
            break;  
        }  
  
        for ( int i = 0; i < number; i++ )  
        {  
            int sockfd = events[i].data.fd;  
            if( ( sockfd == pipefd ) && ( events[i].events & EPOLLIN ) )//父进程有写管道表明有新连接需要该子进程处理  
            {  
                int client = 0;  
                /*父子进程之间的管道读取数据，并将结果保存在变量client中，如果读取成功，则表示有新客户连接到来*/  
                ret = recv( sockfd, ( char* )&client, sizeof( client ), 0 );  
                if( ( ( ret < 0 ) && ( errno != EAGAIN ) ) || ret == 0 ) //？  
                {  
                    continue;  
                }  
                else  
                {  
                    struct sockaddr_in client_address;  
                    socklen_t client_addrlength = sizeof( client_address );  
                    int connfd = accept( m_listenfd, ( struct sockaddr* )&client_address, &client_addrlength );//允许客户连接到该子进程  
                    if ( connfd < 0 )  
                    {  
                        printf( "errno is: %d\n", errno );  
                        continue;  
                    }  
                    addfd( m_epollfd, connfd );//将客户连接添加到该子进程的事件表中  
                    /*模板类T必须实现init方法，以初始化一个客户连接。我们会直接使用connfd来索引逻辑处理对象（T类型的对象），以提高程序的效率*/  
                    users[connfd].init( m_epollfd, connfd, client_address );//任务T中提供的init函数用以初始化客户连接，直接用connfd索引客户对象  
                }  
            }  
            /*下面处理子进程接收到的信号*/  
            else if( ( sockfd == sig_pipefd[0] ) && ( events[i].events & EPOLLIN ) )//有注册信号产生  
            {  
                int sig;  
                char signals[1024];  
                ret = recv( sig_pipefd[0], signals, sizeof( signals ), 0 );  
                if( ret <= 0 )  
                {  
                    continue;  
                }  
                else  
                {  
                    for( int i = 0; i < ret; ++i )  
                    {  
                        switch( signals[i] )  
                        {  
                            case SIGCHLD:  
                            {  
                                pid_t pid;  
                                int stat;  
                                while ( ( pid = waitpid( -1, &stat, WNOHANG ) ) > 0 )//WNOHANG没有孩子直接返回  
                                {  
                                    continue;  
                                }  
                                break;  
                            }  
                            case SIGTERM:  
                            case SIGINT:  
                            {  
                                m_stop = true;//子进程终止标志  
                                break;  
                            }  
                            default:  
                            {  
                                break;  
                            }  
                        }  
                    }  
                }  
            }  
            /*如果是其他可读数据，那么必然是客户请求到来。调用逻辑处理对象的process方法处理它*/  
            else if( events[i].events & EPOLLIN )//客户端有数据发送到来  
            {  
                 users[sockfd].process();//调用任务类T的process处理客户数据  
            }  
            else  
            {  
                continue;  
            }  
        }  
    }  
  
    delete [] users;  
    users = NULL;  
    close( pipefd );  
    //close( m_listenfd );//m_listenfd不能由该子进程关闭而是由创建者函数关闭，子进程退出会自动将m_listenfd计数减一  
    close( m_epollfd );  
}  
  

void processpool ::run_parent()//父进程逻辑  mark
{  
    setup_sig_pipe();//启动信号管道  
  
    addfd( m_epollfd, m_listenfd );//父进程监听m_listenfd  
  
    epoll_event events[ MAX_EVENT_NUMBER ];  
    int sub_process_counter = 0;//子进程计数  
    int new_conn = 1;  
    int number = 0;  
    int ret = -1;  
  
    while( ! m_stop )  
    {  
        number = epoll_wait( m_epollfd, events, MAX_EVENT_NUMBER, -1 );//监听注册事件  
        if ( ( number < 0 ) && ( errno != EINTR ) )  
        {  
            printf( "epoll failure\n" );  
            break;  
        }  
  
        for ( int i = 0; i < number; i++ )  
        {  
            int sockfd = events[i].data.fd;//获取就绪事件描述符  
            if( sockfd == m_listenfd )//监听端口可读表明有新的连接请求  
            {  
                int i =  sub_process_counter;  
                do//通过round robin算法选取一个子进程处理新连接  
                {  
                    if( m_sub_process[i].m_pid != -1 )//为达到最大子进程数###1###  
                    {  
                        break;  
                    }  
                    i = (i+1)%m_process_number;  
                }while( i != sub_process_counter );//  
                  
                if( m_sub_process[i].m_pid == -1 )  
                {  
                    m_stop = true;  
                    break;  
                }  
                sub_process_counter = (i+1)%m_process_number;//子进程计数加一这是round robin算法  
                //send( m_sub_process[sub_process_counter++].m_pipefd[0], ( char* )&new_conn, sizeof( new_conn ), 0 );  
                send( m_sub_process[i].m_pipefd[0], ( char* )&new_conn, sizeof( new_conn ), 0 );//通知子进程有新连接需要处理  
                printf( "send request to child %d\n", i );  
                //sub_process_counter %= m_process_number;  
            }  
            /*下面处理父进程接受到的信号*/  
            else if( ( sockfd == sig_pipefd[0] ) && ( events[i].events & EPOLLIN ) )//有注册信号  
            {  
                int sig;  
                char signals[1024];  
                ret = recv( sig_pipefd[0], signals, sizeof( signals ), 0 );  
                if( ret <= 0 )  
                {  
                    continue;  
                }  
                else  
                {  
                    for( int i = 0; i < ret; ++i )  
                    {  
                        switch( signals[i] )  
                        {  
                            case SIGCHLD:  
                            {  
                                pid_t pid;  
                                int stat;  
                                while ( ( pid = waitpid( -1, &stat, WNOHANG ) ) > 0 )  
                                {  
                                    for( int i = 0; i < m_process_number; ++i )  
                                    {  
                           /*如果进程池中第i个子进程退出了，则主进程关闭相应的通信管道，并设置相应的m_pid为-1，以标记该子进程已经推出了*/  
                                        if( m_sub_process[i].m_pid == pid )  
                                        {  
                                            printf( "child %d join\n", i );  
                                            close( m_sub_process[i].m_pipefd[0] );  
                                            m_sub_process[i].m_pid = -1;//这里置为-1是为了在终止所有子进程之前防止有新的客户连接有调用子进程,he ###1###配合使用  
                                        }  
                                    }  
                                }  
                                //如果所有子进程都退出了，则父进程也退出  
                                m_stop = true;//终止标志  
                                for( int i = 0; i < m_process_number; ++i )  
                                {  
                                    if( m_sub_process[i].m_pid != -1 )//还有子进程没有终止则不能终止父进程  
                                    {  
                                        m_stop = false;  
                                    }  
                                }  
                                break;  
                            }  
                            case SIGTERM:  
                            case SIGINT:  
                            {  
                                /*如果父进程接收到终止信号，那么就杀死所有子进程，并等待它们全部结束。当然，通知子进程结束更好的方法是向父子进程之间的管道发送特殊的数据*/  
                                printf( "kill all the clild now\n" );  
                                for( int i = 0; i < m_process_number; ++i )  
                                {  
                                    int pid = m_sub_process[i].m_pid;  
                                    if( pid != -1 )  
                                    {  
                                        kill( pid, SIGTERM );  
                                    }  
                                }  
                                break;  
                            }  
                            default:  
                            {  
                                break;  
                            }  
                        }  
                    }  
                }  
            }  
            else  
            {  
                continue;  
            }  
        }  
    }  
  
    //close( m_listenfd );//m_listenfd不是父进程创建的不能由他关闭，应该由创建者关闭  
    close( m_epollfd );  
}  
  
