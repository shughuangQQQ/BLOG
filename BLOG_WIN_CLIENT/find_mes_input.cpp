#include "find_mes_input.h"
#include "ui_find_mes_input.h"

Find_Mes_Input::Find_Mes_Input(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Find_Mes_Input)
{
    ui->setupUi(this);
}

Find_Mes_Input::~Find_Mes_Input()
{
    delete ui;
}
