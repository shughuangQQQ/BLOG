#ifndef BLOG_H
#define BLOG_H

#include <QWidget>

namespace Ui {
class Blog;
}

class Blog : public QWidget
{
    Q_OBJECT

public:
    explicit Blog(QWidget *parent = 0);
    ~Blog();

private:
    Ui::Blog *ui;
};

#endif // BLOG_H
