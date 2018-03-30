#ifndef DATARAS_H
#define DATARAS_H

#include <QObject>
#include"q_tcp_util.h"
class DataRAS :public QObject
{
    Q_OBJECT
public:
    DataRAS(Q_TCP_Util *tcpsoc);
    Q_TCP_Util *m_tcp;

    QString m_log_in_id;
    QString m_log_in_passward;
    int SendToSocket(M_PACK*str);

    int RecvFromSocket();
    ~ DataRAS();
public slots:
    BOOL getin();
};

#endif // DATARAS_H
