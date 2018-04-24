#include "q_tcp_util.h"
#include "net_pack.h"
#include "mainwindow.h"
Q_TCP_Util::Q_TCP_Util(QObject *parent) : QObject(parent)
{

}
 Q_TCP_Util* Q_TCP_Util::CreateNetUtil()
{
    Q_TCP_Util* net_util=new Q_TCP_Util;
     net_util->client_socket=new QTcpSocket();
     net_util-> client_socket->connectToHost(QHostAddress(SERVER_IP), SERVER_PORT,QTcpSocket::ReadWrite);
    if(!net_util->client_socket->waitForConnected(1000))
    {
        qDebug() << "Connection failed!";
        return NULL;
    }
     qDebug() << "Connect successfully!";
     return net_util;
}
int Q_TCP_Util::f_send(QString m_pac)
{
  qDebug()<<"111111"<<m_pac;

   char*  ch;
   QByteArray ba = m_pac.toLatin1(); // must
   ch=ba.data();
    if(-1==this->client_socket->write(ch))
    {

        return 0;

    }
    if(this->client_socket->waitForBytesWritten())
    {

        return 1;
    }


    return 1;
}
int Q_TCP_Util::f_recv(char * recvbuf)
{

     if(this->client_socket->waitForReadyRead())
     {
        client_socket->read(recvbuf,4096);
        /* QDataStream in(this->client_socket);
         in.setVersion(QDataStream::Qt_5_0);
         in >> recvbuf;*/
         qDebug()<<recvbuf;

     }
     return 0;


 return 1;
}

 Q_TCP_Util::~Q_TCP_Util()
 {


 }
