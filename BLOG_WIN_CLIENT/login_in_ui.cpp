#include "login_in_ui.h"
#include "ui_login_in_ui.h"

Login_in_UI::Login_in_UI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login_in_UI)
{
 //   setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
   // setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_ShowModal,true);
    ui->setupUi(this);


}

Login_in_UI::~Login_in_UI()
{
    delete ui;
}



void Login_in_UI::on_Login_clicked()
{
    log_id=ui->m_ID->text();
    log_passward=ui->m_passward->text();


    emit sendtoutil();


}

void Login_in_UI::on_SignUp_clicked()
{
    emit show_sign_up_window();
    this->hide();
}
