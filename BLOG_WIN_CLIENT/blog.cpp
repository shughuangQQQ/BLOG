#include "blog.h"
#include "ui_blog.h"

Blog::Blog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Blog)
{
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    ui->setupUi(this);
    this->resize(500,600);
}

Blog::~Blog()
{
    delete ui;
}
