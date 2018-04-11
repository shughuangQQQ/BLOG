#ifndef Q_TCP_UTIL_H
#define Q_TCP_UTIL_H

#include<QtNetwork>
#include "net_pack.h"
#include <QObject>

class Q_TCP_Util : public QObject
{
    Q_OBJECT
private:
    QTcpSocket*client_socket;
public:
     explicit Q_TCP_Util(QObject *parent = nullptr);
    ~Q_TCP_Util();
    int f_send(char *m_pac);
    int f_recv(QJsonObject *m_pac);
public:
    static Q_TCP_Util* CreateNetUtil();



};

#endif // Q_TCP_UTIL_H
