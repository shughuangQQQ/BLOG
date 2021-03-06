#-------------------------------------------------
#
# Project created by QtCreator 2018-03-30T10:53:16
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BLOG_WIN_CLIENT
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    q_tcp_util.cpp \
    ui_flush_util.cpp \
    login_in_ui.cpp \
    dataras.cpp \
    deal_qjson.cpp \
    blog.cpp \
    find_mes_input.cpp \
    signupform.cpp \
    meform.cpp

HEADERS += \
        mainwindow.h \
    q_tcp_util.h \
    net_pack.h \
    ui_flush_util.h \
    login_in_ui.h \
    dataras.h \
    deal_qjson.h \
    blog.h \
    find_mes_input.h \
    signupform.h \
    meform.h

FORMS += \
        mainwindow.ui \
    login_in_ui.ui \
    blog.ui \
    find_mes_input.ui \
    signupform.ui \
    meform.ui

