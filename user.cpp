#include "user.h"

User::User(QTcpSocket * sock,QString account,QString passwd):clientsock(sock),m_account(account),m_passwd(passwd)
{

}
