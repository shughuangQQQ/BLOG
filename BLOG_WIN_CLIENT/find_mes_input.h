#ifndef FIND_MES_INPUT_H
#define FIND_MES_INPUT_H

#include <QWidget>

namespace Ui {
class Find_Mes_Input;
}

class Find_Mes_Input : public QWidget
{
    Q_OBJECT

public:
    explicit Find_Mes_Input(QWidget *parent = 0);
    ~Find_Mes_Input();
    QString find_person_id;
private slots:
    void on_pushButton_clicked();

private:
    Ui::Find_Mes_Input *ui;
signals:
    void SendToServer(QString);
};

#endif // FIND_MES_INPUT_H
