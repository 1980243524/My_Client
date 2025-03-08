#ifndef NETWORKER_H
#define NETWORKER_H

#include <QObject>
#include<QTcpSocket>
#include<QMessageBox>
#include<QThread>
#include"user.h"

struct Require{
    static const size_t HEADLEN;
    struct Head{
        size_t Length;
        quint32 SourceId;
        quint32 DestinationId;
        friend QDataStream& operator<<(QDataStream& out, const Head& head) {
            out << head.Length<<head.SourceId<<head.DestinationId;
            return out;
        }

        // 反序列化函数
        friend QDataStream& operator>>(QDataStream& in, Head& head) {
            in >> head.Length>>head.SourceId>>head.DestinationId;
            return in;
        }
    };
    Head m_head;
    QString m_data="";

    QString operator=(QString message){
        m_data=message;
        m_head.Length=m_data.toUtf8().size();
        return m_data;
    }
    bool operator==(QString message) const {
        return m_data==message;
    }
    QString operator+(QString message)const {
        return m_data+message;
    }
    int size()const  {
        return HEADLEN+m_head.Length;
    }
    friend QDataStream& operator<<(QDataStream& out, const Require& require) {
        QByteArray temp=require.m_data.toUtf8();
        out << require.m_head ;
        out.writeRawData(require.m_data.toUtf8().data(),require.m_head.Length);
        return out;
    }

    // 反序列化函数
    friend QDataStream& operator>>(QDataStream& in, Require& require) {
        in >> require.m_head >> require.m_data;
        return in;
    }
};



struct Response{
    struct Head{
        size_t Length;
        quint32 SourceId;
        quint32 DestinationId;
        quint32 Code;
        Head()=default;
        Head(size_t length,quint32 sourceId,quint32 destinationId,quint32 code)
            :Length(length),SourceId(sourceId),DestinationId(destinationId),Code(code) {}
        friend QDataStream& operator<<(QDataStream& out, const Head& head) {
            out << head.Length<<head.SourceId<<head.DestinationId<<head.Code;
            return out;
        }

        // 反序列化函数
        friend QDataStream& operator>>(QDataStream& in, Head& head) {
            in >> head.Length>>head.SourceId>>head.DestinationId>>head.Code;
            return in;
        }
    };
    Response()=default;
    Response(quint32 sourceId,quint32 destinationId,quint32 code,QString message)
        :m_head(message.size(),sourceId,destinationId,code),m_data(message) {}

    static const size_t HEADLEN;
    Head m_head;
    QString m_data;
    QString operator=(QString message){
        m_data=message;
        m_head.Length=m_data.toUtf8().size();
        return m_data;
    }
    bool operator==(QString message) const {
        return m_data==message;
    }
    QString operator+(QString message)const {
        return m_data+message;
    }
    int size()const  {
        return HEADLEN+m_head.Length;
    }
    friend QDataStream& operator<<(QDataStream& out, const Response& response) {
        out << response.m_head << response.m_data.toUtf8();
        return out;
    }

    // 反序列化函数
    friend QDataStream& operator>>(QDataStream& in, Response& response) {
        in >> response.m_head >> response.m_data;
        return in;
    }
};

void serializeStruct(const Require& require, QByteArray& byteArray);

class Networker : public QObject
{
    Q_OBJECT
public:
    explicit Networker(QObject *parent = nullptr);
    ~Networker();
    User* user;
public slots:
    void ConnectToServer(const QString& ip,quint16 port);
    void Login(const QString& account,const QString& passwd);
    void ProcessSockRead();
    void SendMessage(quint32 sourceId,quint32 desId,QString message);
    void Enroll(const QString& account,const QString& passwd);
    void AddFriend(QString username);
private:
    bool checkConnection(){return clientsock->waitForConnected(0);}
    int ParseResponse(Response& response);


    int Service(Response& response);
private:
    QTcpSocket* clientsock;
    QString m_friendName;

signals:
    void failtoconnect(QString);
    void failtoparseResponse(QString);
    void successfullyLogin(quint32,std::vector<std::pair<QString,quint32>>*,QString);
    void receiveFromPublic(QString);
    void receiveFromEcho(QString);
    void receiveFromEnroll(QString);
    void receiveFromMakeFriend(QString);
    void receiveFromPrivate(QString);
};


#endif // NETWORKER_H
