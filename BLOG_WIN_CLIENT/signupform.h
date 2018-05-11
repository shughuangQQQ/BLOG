#ifndef SIGNUPFORM_H
#define SIGNUPFORM_H

#include <QWidget>

namespace Ui {
class SignUpForm;
}

class SignUpForm : public QWidget
{
    Q_OBJECT

public:
    explicit SignUpForm(QWidget *parent = 0);
    ~SignUpForm();
    int sign_blag;
    QWidget*use;
private slots:
    void on_pushButton_clicked();
    void Is_Avialble();
    void on_pushButton_2_clicked();

private:
    Ui::SignUpForm *ui;
signals:
   void check_aviable();
   void send_sign_request();


};

#endif // SIGNUPFORM_H
