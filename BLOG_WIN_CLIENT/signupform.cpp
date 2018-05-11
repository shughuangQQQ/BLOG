#include "signupform.h"
#include "ui_signupform.h"
#include<QMessageBox>
#include<mainwindow.h>
#include"dataras.h"

SignUpForm::SignUpForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignUpForm)
{
    sign_blag=0;
    setAttribute(Qt::WA_ShowModal,false);
    ui->setupUi(this);
}

SignUpForm::~SignUpForm()
{
    delete ui;
}

void SignUpForm::on_pushButton_clicked()
{
    emit check_aviable();
    if(sign_blag==1)
    {
       MainWindow*mainptr= (MainWindow*)use;
       mainptr->m_util->m_dataras->m_log_in_id=ui->idedit->text();
       mainptr->m_util->m_dataras->m_log_in_passward=ui->pasedit->text();
    emit send_sign_request();
    }
    else
        QMessageBox::about(NULL, "Sign", "Sign Failed,Please insure that ur id&&passward between 6~15!");

}
 void SignUpForm::Is_Avialble()
 {
     if(ui->pasedit->text().length()<6
        ||ui->pasedit->text().length()>15
        ||ui->idedit->text().length()<6
        ||ui->idedit->text().length()>15)
     {

         sign_blag=0;
     }
     else
         sign_blag=1;


 }

void SignUpForm::on_pushButton_2_clicked()
{
    this->hide();
    MainWindow*mainptr= (MainWindow*)use;
    mainptr->m_util->m_logui->show();
}
