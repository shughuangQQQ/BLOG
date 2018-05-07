#include "signupform.h"
#include "ui_signupform.h"

SignUpForm::SignUpForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignUpForm)
{
    ui->setupUi(this);
}

SignUpForm::~SignUpForm()
{
    delete ui;
}
