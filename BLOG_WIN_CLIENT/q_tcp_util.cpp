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
int Q_TCP_Util::f_send(char *m_pac)
{

    if(-1==this->client_socket->write((char *)m_pac,sizeof(m_pac)))
    {
        return 0;

    }
    return 1;
}
int Q_TCP_Util::f_recv(QJsonObject *m_pac)
{
 if(-1==this->client_socket->write((char *)m_pac,sizeof(m_pac)))
 {
     return 0;

 }
 return 1;
}

 Q_TCP_Util::~Q_TCP_Util()
 {


 }
