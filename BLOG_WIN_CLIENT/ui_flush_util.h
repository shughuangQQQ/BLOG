#ifndef UI_FLUSH_UTIL_H
#define UI_FLUSH_UTIL_H

#include <QObject>

#include"blog.h"
#include"login_in_ui.h"
#include"q_tcp_util.h"
#include"dataras.h"
#include"find_mes_input.h"
class UI_FLUSH_UTIL:public QObject
{
     Q_OBJECT

public:
     explicit UI_FLUSH_UTIL(QObject *parent = nullptr);
    Login_in_UI* m_logui;
    Q_TCP_Util *m_tcp;
    DataRAS *m_dataras;
    Find_Mes_Input *m_find;
      ~UI_FLUSH_UTIL();
    Blog*m_blog_ui;
private:
   QString m_id;
   QString m_pass;
signals:
   void sendtoNetUtil();
public slots:
   void getpass();
   void FOCUS_OK();
   void Ui_LOG_CLOSE();
   void ShowBlogChildUi();
   void Show_Find_Widget();

};

#endif // UI_FLUSH_UTIL_H
