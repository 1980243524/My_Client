#ifndef SUBTHREAD_H
#define SUBTHREAD_H

#include <QObject>
#include<QThread>
#include<QString>
#include<QTcpSocket>
QString const SERVER_IP="127.0.0.1";
quint16 const SERVER_PORT=8080;
int const LOGIN_FLAG=1;     //登录功能标志
int const ENROLL_FLAG=2;    //注册功能标志

int const CONNECTION_ERROR=0;       //连接失败错误标志
int const ACCOUNT_NOT_EXIST=1;               //账号错误标志
int const PASSWD_ERROR=2;
int const ACCOUNT_EXIST=3;
class SubThread : public QObject
{
    Q_OBJECT
private:
    QByteArray buffer;

    QTcpSocket* task_connect(int const windowflag);
public:
    explicit SubThread(QObject *parent = nullptr);
    QString m_account;
    QString m_passwd;
    int error_flag=-1;
signals:
    void ERROR(int const widget,int const error_flag);
    void login_done(QTcpSocket *,QString,QString);
public slots:
    void task_login(QString,QString);
    void task_enroll(QString,QString);
};

#endif // SUBTHREAD_H
