#include "ui_flush_util.h"
#include <QMessageBox>
UI_FLUSH_UTIL::UI_FLUSH_UTIL(QObject *parent) : QObject(parent)
{
    m_blog_ui=new Blog((QWidget*)this->parent());
    m_logui=new Login_in_UI;
   m_tcp=Q_TCP_Util::CreateNetUtil();
   m_dataras=new DataRAS(m_tcp);
   this->m_find=new Find_Mes_Input;
    m_blog_ui->hide();
   QObject::connect(m_logui,SIGNAL(sendtoutil()),this,SLOT(getpass()));
   QObject::connect(this,SIGNAL(sendtoNetUtil()),m_dataras,SLOT(getin()));
   QObject::connect(m_dataras,SIGNAL(CloseLogUi()),this,SLOT(Ui_LOG_CLOSE()));
    QObject::connect(this->parent(),SIGNAL(BlogButtonPush()),this,SLOT(ShowBlogChildUi()));
   //QObject::connect(this,SIGNAL(()),m_dataras,SLOT(Ui_LOG_CLOSE()));
   QObject::connect(this->parent(),SIGNAL(Send_Find_User_SIG()),this,SLOT(Show_Find_Widget()));
   QObject::connect(m_find,SIGNAL(SendToServer(QString)),m_dataras,SLOT(Get_PerSon(QString)));

   QObject::connect(m_dataras,SIGNAL(USER_FIND(int)),m_find,SLOT(F_USER_FIND(int)));

  QObject::connect(m_find,SIGNAL(FOUCS_USER(QString)),m_dataras,SLOT(F_USER_FOUCS(QString)));
   QObject::connect(m_dataras,SIGNAL(FOCUS_SEND()),this,SLOT(FOCUS_OK()));

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


void UI_FLUSH_UTIL::Ui_LOG_CLOSE()
{
    if(m_dataras->getinBlag==true)
    {
        m_logui->close();
        delete(m_logui);
        m_logui=NULL;
    }


}
void UI_FLUSH_UTIL::ShowBlogChildUi()
{
    this->m_blog_ui->show();
}
 void UI_FLUSH_UTIL::FOCUS_OK()
 {
      QMessageBox::about(NULL, "FOUCS", "FOUCS OK!");
 }
void UI_FLUSH_UTIL::Show_Find_Widget()
{

    this->m_find->show();
}
