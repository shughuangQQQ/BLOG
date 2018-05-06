#ifndef FIND_MES_INPUT_H
#define FIND_MES_INPUT_H

#include <QWidget>
#include<QTreeWidgetItem>
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
    void F_USER_FIND(int);
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_3_clicked();

    //void on_treeWidget_clicked(const QModelIndex &index);

private:
    int m_line;
    QString m_find_id;
    Ui::Find_Mes_Input *ui;
signals:
    void SendToServer(QString);
    void FOUCS_USER(QString);
};

#endif // FIND_MES_INPUT_H
