#include "deal_qjson.h"

Deal_QJson::Deal_QJson(QObject *parent) : QObject(parent)
{

}
QJsonObject* Deal_QJson::CreateLogJson(QString user_id,QString user_passward)
{
    QJsonObject personMessage;
    QJsonObject *logjson=new QJsonObject;
    personMessage.insert("id",user_id);
    personMessage.insert("passward",user_passward);
    logjson->insert("logmessage",QJsonValue(personMessage));
    return logjson;



}
QString Deal_QJson::JsonToStr(QJsonObject *m_json)
{
    QJsonDocument document;

    document.setObject(*m_json);

    QByteArray byteArray =document.toJson(QJsonDocument::Compact);

    QString strJson(byteArray);

    qDebug() << strJson;

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
