#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QMainWindow>
#include<QTcpSocket>
#include<QObject>
#include<QThread>
#include"user.h"
#include <QHostAddress>
#include<QString>
#include<QDebug>
#include"networker.h"
#include"subthread.h"
namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr,Networker* worker=nullptr);
    ~login();

    QThread * login_thread;         //执行登录任务的线程
    Networker* m_worker;
private:
    Ui::login *ui;
    User* client;

public slots:
    void user_login();
signals:
    void siglogin(const QString& ,const QString&); //触发信号使线程开始执行登录任务
    void open_mainwindow(User);         //触发信号使主界面打开
    void open_enrollwindow();
    void ERROR(int const widget,int const error_flag);
};

#endif // LOGIN_H
