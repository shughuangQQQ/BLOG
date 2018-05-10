#include "meform.h"
#include "ui_meform.h"
#include "mainwindow.h"
MeForm::MeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeForm)
{
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    ui->setupUi(this);
    this->resize(500,600);
     m_fans_num=-1;
     m_focus_num=-1;
     m_blog_item_num=-1;
    QString m_mysunb_mes=QString("null");
}

MeForm::~MeForm()
{
    delete ui;
}
 void MeForm::SetAllMes(int m_fansnum,int m_focusnum,int m_blogitem,QString mysunb_mes)
 {

     m_fans_num=m_fansnum;
     m_focus_num=m_focusnum;
     m_blog_item_num=m_blogitem;
     m_mysunb_mes=mysunb_mes;
 }
void MeForm::me_setmes()
{
    MainWindow*mainclass=(MainWindow*)parent();
    ui->userid->setText(mainclass->m_util->m_id);
    ui->fansnum->setNum(m_fans_num);
    ui->focusnum->setNum(m_focus_num);
    ui->blogitem->setNum(m_blog_item_num);


    QTreeWidgetItem *items = new QTreeWidgetItem(ui->treeWidget,
                                 QStringList(m_mysunb_mes));
   /*

   QString m_mysunb_mes=new QString("null");*/
}

void MeForm::on_pushButton_clicked()
{
    emit Push_Me_Static(ui->lineEdit->text());
}
