#ifndef SUBTHREAD_H
#define SUBTHREAD_H

#include <QObject>
#include<QThread>
#include<QString>
#include<QTcpSocket>
int const CONNECTION_ERROR=0;       //连接失败错误标志
int const ACCOUNT_ERROR=1;               //账号错误标志
class SubThread : public QObject
{
    Q_OBJECT
private:
    QByteArray buffer;
    QTcpSocket *clientsock;
    bool task_connect();
public:
    explicit SubThread(QObject *parent = nullptr);
    QString m_account;
    QString m_passwd;
signals:
    void ERROR(int const);
    void connection_done(QTcpSocket *,QString,QString);
public slots:
    void task_login(QString,QString);

};

#endif // SUBTHREAD_H
