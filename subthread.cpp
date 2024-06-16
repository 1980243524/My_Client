#include "subthread.h"
#include<QTcpSocket>
#include<QtDebug>
#include<unistd.h>

struct message
{
    int *length;
    QByteArray data;
};


SubThread::SubThread(QObject *parent) : QObject(parent)
{

}

QTcpSocket* SubThread::task_connect(int const window_flag)
{
    QTcpSocket* clientsock=new QTcpSocket;
    clientsock->connectToHost(SERVER_IP,SERVER_PORT);

    //等待直到连接成功
    if(!clientsock->waitForConnected())
        {
        delete clientsock;
        emit ERROR(window_flag,CONNECTION_ERROR);
        return nullptr;
        }
    return clientsock;
}

void SubThread::task_login(QString account,QString passwd)
{
    int *error_flag=new int(-1);

    QTcpSocket* clientsock=task_connect(LOGIN_FLAG);
    if(!clientsock)return;
    clientsock->write((char*)&LOGIN_FLAG,sizeof(LOGIN_FLAG));

    message m;
    m.length=new int(0);

    *m.length=account.size();
    clientsock->write((char*)m.length,sizeof(int));
    m.data=account.toUtf8();
    clientsock->write(m.data);

    *m.length=passwd.size();
    clientsock->write((char*)m.length,sizeof(int));
    m.data=passwd.toUtf8();
    clientsock->write(m.data);


    clientsock->waitForBytesWritten();
    clientsock->waitForReadyRead();
    clientsock->readLine((char *)error_flag,sizeof(error_flag));


    emit ERROR(LOGIN_FLAG,*error_flag);
    if(*error_flag>0) return ;
    emit login_done(clientsock,account,passwd);

}

void SubThread::task_enroll(QString account,QString passwd)
{
    int *error_flag=new int(-1);

    QTcpSocket* clientsock=task_connect(ENROLL_FLAG);
    if(!clientsock)return;
    clientsock->write((char*)&ENROLL_FLAG,sizeof(ENROLL_FLAG));

    message m;
    m.length=new int(0);

    *m.length=account.size();
    clientsock->write((char*)m.length,sizeof(int));

    m.data=account.toUtf8();
    clientsock->write(m.data);

    *m.length=passwd.size();
    clientsock->write((char*)m.length,sizeof(int));
    m.data=passwd.toUtf8();
    clientsock->write(m.data);

    clientsock->waitForBytesWritten();
    clientsock->waitForReadyRead();
    clientsock->readLine((char *)error_flag,sizeof(error_flag));

    emit ERROR(ENROLL_FLAG,*error_flag);
    if(*error_flag>0) return ;
    clientsock->close();
}

