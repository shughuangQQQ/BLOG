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


     QString m_recvbuf=this->RecvFromSocket();


    QJsonDocument document;

    Deal_QJson::StrToJson(m_recvbuf,&document);
    QJsonObject object=document.object();
    if (object.contains("PACK_TYPE"))
    {
            QJsonValue value = object.value("PACK_TYPE");
            if (value.isString()) {
                QString strType = value.toString();
                qDebug() << "pack_TYPE : " << strType;
            }

    }
    int nFrom=1;
    if (object.contains("PACK_CODE_FEED")) {
        QJsonValue value = object.value("PACK_CODE_FEED");
        if (value.isDouble()) {
            nFrom = value.toVariant().toInt();
            qDebug() << "PACK_CODE_FEED : " << nFrom;
        }
    }
    if(nFrom==LOG_SUCCESS)
        return TRUE;
 }
 DataRAS::~DataRAS()
 {

 }


 int DataRAS::SendToSocket(QString s_mes)
 {

    /* char*  ch;
     QByteArray ba = s_mes.toLatin1(); // must
     ch=ba.data();*/

    if(1== this->m_tcp->f_send(s_mes))
    {
        qDebug()<<"send ok";
        return 1;
    }
    else
      qDebug()<<"send error";
     return 0;
 }
 QString DataRAS::RecvFromSocket()
 {
    char str[4096];
    this->m_tcp->f_recv(str);
    QString qstr = QString(QLatin1String(str));
   /*  QString qstr = QString(QLatin1String(recvbuf));
     QJsonObject*my_json=Deal_QJson::StrToJson(qstr);*/
    for(int i=0;i<4096;i++)
        str[i]='\0';
     qDebug()<<qstr;
     return qstr;
 }
