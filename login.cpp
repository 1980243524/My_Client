#include "login.h"
#include "ui_login.h"





login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    ui->edit_passwd->setEchoMode(QLineEdit::Password);

    login_thread=new QThread;
    SubThread *subt= new SubThread;
    subt->moveToThread(login_thread);

    connect(login_thread,&QThread::finished,subt,&SubThread::deleteLater);
    connect(ui->button_log_on,&QPushButton::clicked,this,&login::user_login);

    connect(this,&login::siglogin,subt,&SubThread::task_login);

    connect(subt,&SubThread::ERROR,this,&login::ERROR);
    connect(subt,&SubThread::login_done,this,&login::get_socket);
    connect(ui->button_enroll,&QPushButton::clicked,this,&login::to_enroll);
}

login::~login()
{
    delete ui;
    login_thread->deleteLater();
}

void login::user_login()
{
    QString account=ui->edit_account->text();
    QString passwd=ui->edit_passwd->text();

    login_thread->start();
    emit siglogin(account,passwd);

}



void login::get_socket(QTcpSocket * sock,QString account,QString passwd)
{
    this->close();
    emit open_mainwindow(User(sock,account,passwd));
}

void login::to_enroll()
{
    this->close();
    emit open_enrollwindow();
}
