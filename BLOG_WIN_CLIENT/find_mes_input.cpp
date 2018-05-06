<<<<<<< HEAD
=======
#include "find_mes_input.h"
#include "ui_find_mes_input.h"
#include"net_pack.h"
Find_Mes_Input::Find_Mes_Input(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Find_Mes_Input)
{
    ui->setupUi(this);
    ui->treeWidget->setHeaderHidden(true);
    ui->pushButton_3->hide();
}

Find_Mes_Input::~Find_Mes_Input()
{
    delete ui;
}

void Find_Mes_Input::on_pushButton_clicked()
{
    emit SendToServer(ui->find_line_edit->text());
}
 void Find_Mes_Input::F_USER_FIND(int find_code)
 {
     if(find_code==ID_EXIST)
     {
         QTreeWidgetItem *items = new QTreeWidgetItem(ui->treeWidget,
                                      QStringList(ui->find_line_edit->text()));

     }
     else
     {
         QTreeWidgetItem *items = new QTreeWidgetItem(ui->treeWidget,
                                       QStringList(QString("failed:id un exist")));
     }
 }

void Find_Mes_Input::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    m_line=column;
    ui->pushButton_3->show();

}

void Find_Mes_Input::on_pushButton_3_clicked()
{
    QTreeWidgetItem*currentwitem=ui->treeWidget->currentItem();

    emit FOUCS_USER(currentwitem->text(m_line));
}

/*void Find_Mes_Input::on_treeWidget_clicked(const QModelIndex &index)
{

}*/
>>>>>>> 4f5da78201977928f90c765dcf1265f651b9f350
