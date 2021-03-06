#ifndef DATARAS_H
#define DATARAS_H

#include <QObject>
#include<qjsonobject.h>

#include"q_tcp_util.h"

class DataRAS :public QObject
{
    Q_OBJECT
public:
    DataRAS(Q_TCP_Util *tcpsoc);
    Q_TCP_Util *m_tcp;
   // Deal_QJson m_dealjson;
    QString m_log_in_id;
    QString m_log_in_passward;
    int SendToSocket(QString);

    QString RecvFromSocket();
    ~ DataRAS();
public:
    bool getinBlag;
public slots:
    BOOL getin();
    BOOL Get_PerSon(QString);
    void F_USER_FOUCS(QString);
   void RequestSelfMes();
  void Push_Person_Static(QString);
  void Sign_Request();
  void Head_Upload(QString);
private:
signals:
  void hide_sign_window();
    void FOCUS_SEND();
    void CloseLogUi();
    void USER_FIND(int);
   void sendtoMeForm(int,int,int,QString);


};

#endif // DATARAS_H
