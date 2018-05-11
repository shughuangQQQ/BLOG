#include "dataras.h"
#include"deal_qjson.h"
#include<QMessageBox>
DataRAS::DataRAS(Q_TCP_Util *tcpsoc)
{
    this->m_tcp=tcpsoc;
    getinBlag=false;
}
 BOOL DataRAS::Get_PerSon(QString findId)
 {
      QJsonObject *findjson=Deal_QJson::CreateFindJson(findId);
      QString sendbuf=Deal_QJson::JsonToStr(findjson);
       qDebug()<<sendbuf;
      this->SendToSocket(sendbuf);
      delete findjson;
      findjson=NULL;


      QString m_recvbuf=this->RecvFromSocket();


     QJsonDocument document;

     Deal_QJson::StrToJson(m_recvbuf,&document);
     QJsonObject object=document.object();
   /*  if (object.contains("PACK_TYPE"))
     {
             QJsonValue value = object.value("PACK_CODE_FEED");
             if (value.isString()) {
                 QString strType = value.toString();
                 qDebug() << "PACK_CODE_FEED : " << strType;
                 if(strType=="")
             }

     }*/
     int nFrom=0;
     if (object.contains("PACK_CODE_FEED")) {
         QJsonValue value = object.value("PACK_CODE_FEED");
         if (value.isDouble()) {
             nFrom = value.toVariant().toInt();
             qDebug() << "PACK_CODE_FEED : " << nFrom;
         }
     }

        emit USER_FIND(nFrom);

}
 void DataRAS::Sign_Request()
 {
     QJsonObject *Sign_Json=Deal_QJson::CreateSignJson(this->m_log_in_id,this->m_log_in_passward);
     QString sendbuf=Deal_QJson::JsonToStr(Sign_Json);
     this->SendToSocket(sendbuf);
     delete Sign_Json;
     Sign_Json=NULL;


     QString m_recvbuf=this->RecvFromSocket();


    QJsonDocument document;

    Deal_QJson::StrToJson(m_recvbuf,&document);
    QJsonObject object=document.object();


    int nFrom=-1;
    if (object.contains("PACK_CODE_FEED"))
    {

            QJsonValue value = object.value("PACK_CODE_FEED");
            if (value.isDouble()) {
                nFrom = value.toVariant().toInt();
                qDebug() << "PACK_CODE_FEED : " << nFrom;
            }
    }
    if(nFrom==SIGN_UP_SUCCESS)
    {
        QMessageBox::about(NULL, "SIGN", "SIGN OK!");
        emit hide_sign_window();
    }
    else if(nFrom==ID_EXIST)
    {
        QMessageBox::about(NULL, "SIGN", "FAILED:ID exists");
    }


 }
  DataRAS::~DataRAS()
  {

  }
  void  DataRAS::Push_Person_Static(QString push_mes)
  {
       QJsonObject *Push_Static_Json=Deal_QJson::CreatePush_StaticJson(push_mes,this->m_log_in_id);
         QString sendbuf=Deal_QJson::JsonToStr(Push_Static_Json);
         this->SendToSocket(sendbuf);
         delete Push_Static_Json;
         Push_Static_Json=NULL;
         QMessageBox::about(NULL, "Push", "Push OK!");

  }
void DataRAS::RequestSelfMes()
{
    QJsonObject *MeRequestJson=Deal_QJson::CreateRequestMeMesJson(m_log_in_id);
    QString sendbuf=Deal_QJson::JsonToStr(MeRequestJson);
     qDebug()<<sendbuf;
    this->SendToSocket(sendbuf);
    delete MeRequestJson;
    MeRequestJson=NULL;


    QString m_recvbuf=this->RecvFromSocket();


   QJsonDocument document;

   Deal_QJson::StrToJson(m_recvbuf,&document);
   QJsonObject object=document.object();
   int fans_num;
   int friend_num;
   int blogitem_num;
   QString Brief;

   if (object.contains("PACK_BODY"))
   {
           QJsonValue value = object.value("PACK_BODY");

           if (value.isObject()) {
               QJsonObject m_obj=value.toObject();
                    if(m_obj.contains("fans_num"))
                    {
                         QJsonValue value0 = m_obj.value("fans_num");
                         if (value0.isDouble()) {
                             fans_num = value0.toVariant().toInt();

                         }
                    }
                    if(m_obj.contains("friend_num"))
                    {
                        QJsonValue value1 = m_obj.value("friend_num");
                        if (value1.isDouble()) {
                            friend_num = value1.toVariant().toInt();

                        }
                    }
                    if(m_obj.contains("blogitem_num"))
                    {
                        QJsonValue value2 = m_obj.value("blogitem_num");
                        if (value2.isDouble()) {
                            blogitem_num = value2.toVariant().toInt();

                        }
                    }
                    if(m_obj.contains("Brief"))
                    {
                        QJsonValue value3 = m_obj.value("Brief");
                        if (value3.isString()) {
                            Brief = value3.toString();

                        }
                    }
           }
           /* if (value.isString())
            {
                qDebug()<<value.toString();


            }*/
   }
   /*int nFrom=0;
   if (object.contains("PACK_CODE_FEED")) {
       QJsonValue value = object.value("PACK_CODE_FEED");
       if (value.isDouble()) {
           nFrom = value.toVariant().toInt();
           qDebug() << "PACK_CODE_FEED : " << nFrom;
       }
   }*/
    emit sendtoMeForm(fans_num,friend_num,blogitem_num,Brief);

}
 void DataRAS::F_USER_FOUCS(QString foucus_id)
 {
  QJsonObject *Foucs_Json=Deal_QJson::CreateFoucsJson(foucus_id,m_log_in_id);
   QString sendbuf=Deal_QJson::JsonToStr(Foucs_Json);
   this->SendToSocket(sendbuf);
   delete Foucs_Json;
   Foucs_Json=NULL;
   emit FOCUS_SEND();
    //emit

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
    {


        this->getinBlag=true;
         emit CloseLogUi();
         return TRUE;
    }
 }

  void DataRAS::Head_Upload(QString pix_address)
  {

      QJsonObject *Head_Pix_Json=Deal_QJson::CreateHeadPixJson(pix_address,m_log_in_id);
       if(Head_Pix_Json==NULL)
       {
            QMessageBox::about(NULL, "HEAD_SET", "HEAD IS TOO BIG Please keep pix less than 1m!");
           return;
       }
      QString sendbuf=Deal_QJson::JsonToStr(Head_Pix_Json);

      this->SendToSocket(sendbuf);
      delete Head_Pix_Json;
      Head_Pix_Json=NULL;



  }
 QString DataRAS::RecvFromSocket()
 {
    char str[4096];
    for(int i=0;i<4096;i++)
        str[i]='\0';
    this->m_tcp->f_recv(str);
    QString qstr = QString(QLatin1String(str));
   /*  QString qstr = QString(QLatin1String(recvbuf));
     QJsonObject*my_json=Deal_QJson::StrToJson(qstr);*/
    for(int i=0;i<4096;i++)
        str[i]='\0';
     qDebug()<<qstr;
     return qstr;
 }
