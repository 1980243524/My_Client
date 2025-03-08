#include "networker.h"

const size_t Require::HEADLEN=16;
const size_t Response::HEADLEN=20;

void serializeStruct(const Require& require, QByteArray& byteArray) {
    QDataStream out(&byteArray, QIODevice::WriteOnly);
    out.writeRawData((char*)&require.m_head,Require::HEADLEN);
    out.writeRawData(require.m_data.toUtf8().data(),require.m_head.Length);
    //out << require;
}

Networker::Networker(QObject *parent)
    : QObject{parent},
    user(new User),
    clientsock(new QTcpSocket(this))
{
    connect(clientsock,&QTcpSocket::readyRead,this,&Networker::ProcessSockRead);
}
void Networker::ConnectToServer(const QString& ip,quint16 port){
        clientsock->connectToHost(ip,port);
}

int Networker::ParseResponse(Response& response){
    int ret=clientsock->read((char*)&response.m_head,response.HEADLEN);

    //response.m_data.resize(response.m_head.Length);
    if(ret<=0) return -1;
    QByteArray buffer(response.m_head.Length,'\0');

    size_t received = 0;
    while (received < response.m_head.Length) {
        ssize_t ret=clientsock->read(buffer.data()+received,response.HEADLEN);
        if (ret <= 0) {
            // 处理错误（ret == 0：连接关闭；ret < 0：错误）
            return -1;
        }
        received += ret;
    }
    response.m_data=QString::fromUtf8(buffer);
    return 0;
}

void Networker::Login(const QString& account,const QString& passwd){
    if(!clientsock->waitForConnected(0)){
        emit failtoconnect("无法连接服务器");
        ConnectToServer("127.0.0.1",8080);
        return;
    }
    user->m_account=account;
    user->m_passwd=passwd;
    SendMessage(1,1,account+'\\'+passwd);
}

void Networker::ProcessSockRead(){

    Response response;
    int ret=ParseResponse(response);
    if(ret<0){
        emit failtoparseResponse("服务器响应解析失败");
    }
    if(response.m_head.SourceId<100){
        qDebug()<<response.m_data;
        if(response.m_head.Code==500){
            return ;
        }
        user->m_id=response.m_head.DestinationId;
        Service(response);
        return;
    }
    emit receiveFromPrivate(response.m_data);
}

int Networker::Service(Response& response){
    switch (response.m_head.SourceId)
    {
    case 1:
        quint32 n;
        quint64 ret;
        clientsock->waitForReadyRead();
        ret=clientsock->read((char*)&n,sizeof(quint32));
        for(quint32 i=0;i<n;i++){
            Response temp;
            ParseResponse(temp);
            user->friends.push_back({temp.m_data,temp.m_head.SourceId});
        }
        emit successfullyLogin(response.m_head.DestinationId,&user->friends,user->m_account);
        break;
    case 2:
        emit receiveFromEnroll(response.m_data);
        break;
    case 3:
        emit receiveFromEcho(response.m_data);
        break;
    case 4:
        emit receiveFromPublic(response.m_data);
        break;
    case 6:
        user->friends.push_back({m_friendName,response.m_data.toUInt()});
        emit receiveFromMakeFriend(response.m_data);
        break;
    //default:
    }
    return 0;
}
Networker::~Networker(){
    delete  user;
    delete  clientsock;
}

void Networker::SendMessage(quint32 sourceId,quint32 desId,QString message){

    Require require;
    require=message;
    require.m_head.SourceId=sourceId;
    require.m_head.DestinationId=desId;
    QByteArray buffer;
    serializeStruct(require,buffer);
    clientsock->write(buffer);
}

void Networker::Enroll(const QString& account,const QString& passwd){
    if(!clientsock->waitForConnected(0)){
        emit failtoconnect("无法连接服务器");
        ConnectToServer("127.0.0.1",8080);
        return;
    }
    SendMessage(2,2,account+'\\'+passwd);
}

void Networker::AddFriend(QString username){
    SendMessage(user->m_id,6,username);
    m_friendName=username;
}
