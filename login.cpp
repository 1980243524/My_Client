#include "login.h"
#include "ui_login.h"
#include <QHostAddress>
#include<QString>
#include<QDebug>
#include<QErrorMessage>



login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    ui->edit_passwd->setEchoMode(QLineEdit::Password);
    clientsock=new QTcpSocket(this);

    login_thread=new QThread;
    SubThread *subt= new SubThread;
    subt->moveToThread(login_thread);
    connect(login_thread,&QThread::finished,subt,&SubThread::deleteLater);
    connect(ui->button_log_on,&QPushButton::clicked,this,&login::log_on);

    connect(this,&login::siglogin,subt,&SubThread::task_login);

    connect(subt,&SubThread::ERROR,this,&login::show_error);
    connect(subt,&SubThread::connection_done,this,&login::get_socket);
}

login::~login()
{
    delete ui;
    login_thread->deleteLater();
}

bool login::log_on()
{
    QString account=ui->edit_account->text();
    QString passwd=ui->edit_passwd->text();

    login_thread->start();
    emit siglogin(account,passwd);

    return true;
}

void login::show_error(int const FLAG)
{
    QErrorMessage *error=new QErrorMessage(this);
    switch(FLAG)
    {
    case 0:
        error->showMessage("连接不到服务器");
        break;
    case 1:
        error->showMessage("未知账号");
        break;
    case 2:
        error->showMessage("密码错误");
        break;
    default:
        break;
    }
}

void login::get_socket(QTcpSocket * sock,QString account,QString passwd)
{
    this->close();
    emit open_mainwindow(User(sock,account,passwd));
}
