#ifndef MEFORM_H
#define MEFORM_H

#include <QWidget>

namespace Ui {
class MeForm;
}

class MeForm : public QWidget
{
    Q_OBJECT

public:
    explicit MeForm(QWidget *parent = 0);
    ~MeForm();

private:
    Ui::MeForm *ui;
private slots:
    void me_setmes();
    void SetAllMes(int,int,int,QString);
public:
    int m_fans_num;
    int m_focus_num;
    int m_blog_item_num;
    QString m_mysunb_mes;

};

#endif // MEFORM_H
