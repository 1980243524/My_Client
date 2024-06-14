#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QMainWindow>
#include<QTcpSocket>
#include<QObject>
#include"subthread.h"
#include"user.h"

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

    QThread * login_thread;         //执行登入任务的线程
private:
    Ui::login *ui;

    QString m_account;              //用户账号
    QString m_passwd;               //用户密码

public slots:
    void get_socket(QTcpSocket *,QString,QString); //获取套接字及用户信息,关闭登录界面打开新界面
    void to_enroll();
    void user_login();
signals:
    void siglogin(QString,QString); //触发信号使线程开始执行登录任务
    void open_mainwindow(User);         //触发信号使主界面打开
    void open_enrollwindow();
    void ERROR(int const widget,int const error_flag);
};

#endif // LOGIN_H
