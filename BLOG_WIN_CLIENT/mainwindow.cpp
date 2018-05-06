#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(500,700);
    m_util=new UI_FLUSH_UTIL(this);



}

MainWindow::~MainWindow()
{
    if(m_util)
         delete m_util;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    emit BlogButtonPush();
}

void MainWindow::on_pushButton_3_clicked()
{
    emit Send_Find_User_SIG();
}
