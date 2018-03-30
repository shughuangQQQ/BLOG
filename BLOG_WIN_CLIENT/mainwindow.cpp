#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_util=new UI_FLUSH_UTIL;



}

MainWindow::~MainWindow()
{
    if(m_util)
         delete m_util;
    delete ui;
}
