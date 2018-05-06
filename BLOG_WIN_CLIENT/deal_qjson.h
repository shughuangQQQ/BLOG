#ifndef DEAL_QJSON_H
#define DEAL_QJSON_H

#include <QObject>
#include<qjsonobject.h>
#include<qjsondocument.h>
#include<qdebug.h>
class Deal_QJson : public QObject
{
    Q_OBJECT
public:
    explicit Deal_QJson(QObject *parent = nullptr);
public:
     static QJsonObject* CreateLogJson(QString,QString);
     static  QString JsonToStr(QJsonObject* m_json);
     static int StrToJson(QString m_recvbuf,QJsonDocument*jsonDoc);
     static QJsonObject* CreateFindJson(QString);

     static QJsonObject* CreateFoucsJson(QString,QString);
signals:

public slots:
};

#endif // DEAL_QJSON_H
