#include "deal_qjson.h"
#include<QFile>
#include<QDebug>


Deal_QJson::Deal_QJson(QObject *parent) : QObject(parent)
{

}
QJsonObject* Deal_QJson::CreateFindJson(QString find_id)
{

     QJsonObject JsonPack;
    QJsonObject *findjson=new QJsonObject;
    JsonPack.insert("find",find_id);
    findjson->insert("PACK_TYPE","FIND");
    findjson->insert("FIND_MES",QJsonValue(JsonPack));

   return findjson;
}
/*QJsonObject *Deal_QJson::CreateHeadPixPack(QString pix_address,QString userid)
{

   QFile file(pix_address);
    if(file.size()>1024*1024)
    {
        return NULL;
    }
        //判断文件是否存在
        if(file.exists()){
            qDebug()<<"file exist";
        }else{
            qDebug()<<"file un exist";
        }
        //已读写方式打开文件，
        //如果文件不存在会自动创建文件
        if(!file.open(QIODevice::ReadOnly)){
            qDebug()<<"open failed";
        }else{
            qDebug()<<"open success";
        }

        //读取文件


        QByteArray QBA=file.readAll();

        file.close();
         char *ch=QBA.data();

        QString string;
        string = QString(QBA);


        QJsonObject *head_pix_json=new QJsonObject;





        head_pix_json->insert("PACK_TYPE","HEAD_SET");
        head_pix_json->insert("id",userid);
        head_pix_json->insert("head_pix",);
        return head_pix_json;

}*/
QJsonObject *Deal_QJson::CreateSignJson(QString userid,QString userpassward)
{

    QJsonObject *signjson=new QJsonObject;


    signjson->insert("PACK_TYPE","SIGN_UP");
    signjson->insert("id",userid);
    signjson->insert("passward",userpassward);
    return signjson;
}
 QJsonObject* Deal_QJson::CreatePush_StaticJson(QString push_mes,QString m_id)
{

     QJsonObject *pushjson=new QJsonObject;


     pushjson->insert("PACK_TYPE","push_static");
     pushjson->insert("id",m_id);
     pushjson->insert("push_mes",push_mes);
     return pushjson;

}
 QJsonObject* Deal_QJson::CreateFoucsJson(QString foucus_id,QString m_id)
{

    QJsonObject *foucus_json=new QJsonObject;


    foucus_json->insert("PACK_TYPE","FOCUS");
    foucus_json->insert("id",m_id);
    foucus_json->insert("focus_id",foucus_id);
    return foucus_json;

}
QJsonObject* Deal_QJson::CreateRequestMeMesJson(QString user_id)
{

    QJsonObject *personMessage=new QJsonObject;
     personMessage->insert("PACK_TYPE","MeMes");
    personMessage->insert("id",user_id);




    return personMessage;

}
QJsonObject* Deal_QJson::CreateLogJson(QString user_id,QString user_passward)
{
    QJsonObject personMessage;
    QJsonObject *logjson=new QJsonObject;
    personMessage.insert("id",user_id);
    personMessage.insert("passward",user_passward);
    logjson->insert("PACK_TYPE","login");
    logjson->insert("User_Mes",QJsonValue(personMessage));

    return logjson;



}
int Deal_QJson::StrToJson(QString m_recvbuf,QJsonDocument*jsonDoc)
{
    QJsonParseError jsonError;

    *jsonDoc=QJsonDocument::fromJson(m_recvbuf.toLatin1(), &jsonError);

  if(jsonError.error == QJsonParseError::NoError)
    {
        if((*jsonDoc).isObject())
        {

           return 1;
        }

    }
    return 0;

}
QString Deal_QJson::JsonToStr(QJsonObject *m_json)
{
    QJsonDocument document;

    document.setObject(*m_json);

    QByteArray byteArray =document.toJson(QJsonDocument::Compact);

    QString strJson(byteArray);

   // qDebug() << strJson;

    //char*ch;
    //QByteArray ba = strJson.toLatin1();
    //ch=ba.data();
    return strJson;

}
/*
// QString >> QJson
QJsonObject getJsonObjectFromString(const QString jsonString){
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toLocal8Bit().data());
    if( jsonDocument.isNull() ){
        qDebug()<< "===> please check the string "<< jsonString.toLocal8Bit().data();
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}
// QJson >> QString
QString getStringFromJsonObject(const QJsonObject& jsonObject){
    return QString(QJsonDocument(jsonObject).toJson());
}
*/
