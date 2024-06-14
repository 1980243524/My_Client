#include "subthread.h"
#include<QTcpSocket>
#include<QtDebug>
#include<unistd.h>



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
    error_flag=-1;
    QTcpSocket* clientsock=task_connect(LOGIN_FLAG);
    if(!clientsock)return;

    buffer=(QString::number(account.size())+'#'+account+passwd).toUtf8();
    clientsock->write(buffer);

    clientsock->waitForBytesWritten();

    clientsock->waitForReadyRead();

    clientsock->readLine((char*)&error_flag,sizeof(error_flag)+1);

    clientsock->write("ok");

    emit ERROR(LOGIN_FLAG,error_flag);
    if(error_flag>0) return ;
    emit connection_done(clientsock,account,passwd);

}

void SubThread::task_enroll()
{
    if(!task_connect(ENROLL_FLAG))return;

}

