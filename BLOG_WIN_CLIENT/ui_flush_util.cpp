#include "ui_flush_util.h"

UI_FLUSH_UTIL::UI_FLUSH_UTIL(QObject *parent) : QObject(parent)
{
    m_logui=new Login_in_UI;
   m_tcp=Q_TCP_Util::CreateNetUtil();
   m_dataras=new DataRAS(m_tcp);
   QObject::connect(m_logui,SIGNAL(sendtoutil()),this,SLOT(getpass()));
   QObject::connect(this,SIGNAL(sendtoNetUtil()),m_dataras,SLOT(getin()));
}

 UI_FLUSH_UTIL::~UI_FLUSH_UTIL()
 {
     if(m_tcp)
         delete m_tcp;
     if(m_logui)
         delete m_logui;


 }
void UI_FLUSH_UTIL::getpass()
{
    m_dataras->m_log_in_id=m_logui->log_id;
    m_dataras->m_log_in_passward=m_logui->log_passward;
    qDebug() << "get pass!";
    emit sendtoNetUtil();

}

