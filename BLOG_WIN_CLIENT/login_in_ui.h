#ifndef LOGIN_IN_UI_H
#define LOGIN_IN_UI_H

#include <QWidget>
#include<qdebug.h>

namespace Ui {
class Login_in_UI;
}

class Login_in_UI : public QWidget
{
    Q_OBJECT

public:
    explicit Login_in_UI(QWidget *parent = 0);
    ~Login_in_UI();
    QString log_id;
     QString log_passward;
private slots:

    void on_Login_clicked();

    void on_SignUp_clicked();

signals:
    void sendtoutil();
   void  show_sign_up_window();
private:

    Ui::Login_in_UI *ui;
};

#endif // LOGIN_IN_UI_H
