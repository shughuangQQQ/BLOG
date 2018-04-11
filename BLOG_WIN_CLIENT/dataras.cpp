#include "dataras.h"
#include"deal_qjson.h"
DataRAS::DataRAS(Q_TCP_Util *tcpsoc)
{
this->m_tcp=tcpsoc;
}
 BOOL DataRAS:: getin()
 {
   // M_PACK* m_pac=new M_PACK;

     QJsonObject *logjson;
     logjson=Deal_QJson::CreateLogJson(m_log_in_id,m_log_in_passward);
    //m_pac->pack_type=PACK_LOG;
     QString sendbuf=Deal_QJson::JsonToStr(logjson);

     this->SendToSocket(sendbuf);
     delete logjson;
     logjson=NULL;

    return TRUE;
 }
 DataRAS::~DataRAS()
 {

 }


 int DataRAS::SendToSocket(QString s_mes)
 {

     char*  ch;
     QByteArray ba = s_mes.toLatin1(); // must
     ch=ba.data();

    if(1== this->m_tcp->f_send(ch))
    {
        qDebug()<<"send ok";
        return 1;
    }
    else
      qDebug()<<"send error";
     return 0;
 }
 int DataRAS::RecvFromSocket(QJsonObject *m_json)
 {

     return 0;
 }
