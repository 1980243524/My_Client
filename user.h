#ifndef USER_H
#define USER_H
#include<QTcpSocket>
#include<vector>
class User
{
public:
    User(QString account,QString passwd);
    User(){}
    QString m_account;              //用户账号
    QString m_passwd;               //用户密码
    quint32 m_id;
    std::vector<std::pair<QString,quint32>> friends;
};

#endif // USER_H
