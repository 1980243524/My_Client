#ifndef USER_H
#define USER_H
#include<QTcpSocket>

class User
{
public:
    User(QTcpSocket * sock,QString account,QString passwd);
    User(){}
    QTcpSocket * clientsock;
    QString m_account;              //用户账号
    QString m_passwd;               //用户密码
};

#endif // USER_H
