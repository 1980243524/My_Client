#include "subthread.h"
#include<QTcpSocket>
#include<QtDebug>
#include<unistd.h>

QString const SERVER_IP="192.168.186.128";
quint16 const SERVER_PORT=9006;

SubThread::SubThread(QObject *parent) : QObject(parent)
{

}

bool SubThread::task_connect()
{
    clientsock=new QTcpSocket;
    clientsock->connectToHost(SERVER_IP,SERVER_PORT);

    //等待直到连接成功
    if(!clientsock->waitForConnected())
        {
        delete clientsock;
        emit ERROR(CONNECTION_ERROR);
        return false;
        }
    return true;
}

void SubThread::task_login(QString account,QString passwd)
{

    if(!task_connect())return;

    buffer=(QString::number(account.size())+'#'+account+passwd).toUtf8();
    clientsock->write(buffer);

    if (!clientsock->waitForBytesWritten())
    {
        qDebug() << "Failed to write data to server.";
        return ;
    }

    if (!clientsock->waitForReadyRead())
    {
        qDebug() << "Failed to read response from server.";
        return ;
    }
    int x;

    clientsock->readLine((char*)&x,sizeof(x)+1);


    clientsock->write("ok");

    emit ERROR(x);
    if(x>0) return ;
    emit connection_done(clientsock,account,passwd);

}


