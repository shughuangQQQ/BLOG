#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include"net_pack.h"
#include"ui_flush_util.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    UI_FLUSH_UTIL* m_util;
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
