#ifndef UI_FLUSH_UTIL_H
#define UI_FLUSH_UTIL_H

#include <QObject>

#include"blog.h"
#include"login_in_ui.h"
#include"q_tcp_util.h"
#include"dataras.h"
#include"find_mes_input.h"
#include"meform.h"
class UI_FLUSH_UTIL:public QObject
{
     Q_OBJECT

public:
     explicit UI_FLUSH_UTIL(QObject *parent = nullptr);
    Login_in_UI* m_logui;
    Q_TCP_Util *m_tcp;
    DataRAS *m_dataras;
    Find_Mes_Input *m_find;
    MeForm* m_meui;
      ~UI_FLUSH_UTIL();
    Blog*m_blog_ui;
      QString m_id;
private:

   QString m_pass;
signals:
   void sendtoNetUtil();
public slots:
   void ShowMeForm();
   void getpass();
   void FOCUS_OK();
   void Ui_LOG_CLOSE();
   void ShowBlogChildUi();
   void Show_Find_Widget();
    void Show_Sign_Widget();

};

#endif // UI_FLUSH_UTIL_H
