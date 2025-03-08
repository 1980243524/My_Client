#include "login.h"
#include "ui_login.h"





login::login(QWidget *parent,Networker* worker) :
    QDialog(parent),
    m_worker(worker),
    ui(new Ui::login)
{
    ui->setupUi(this);
    ui->edit_passwd->setEchoMode(QLineEdit::Password);

    connect(ui->button_log_on,&QPushButton::clicked,this,&login::user_login);

    connect(this,&login::siglogin,worker,&Networker::Login);

    connect(worker,&Networker::failtoconnect,this,[this](QString message){
        QMessageBox::warning(this,"错误",message);
    });//tcp连接建立失败

    //connect(subt,&SubThread::ERROR,this,&login::ERROR);
    //connect(subt,&SubThread::login_done,this,&login::get_socket);
    connect(ui->button_enroll,&QPushButton::clicked,this,[this](){
        this->close();
        emit open_enrollwindow();
    });
}

login::~login()
{
    delete ui;
    //login_thread->deleteLater();
}

void login::user_login()
{
    QString account=ui->edit_account->text();
    QString passwd=ui->edit_passwd->text();

    emit siglogin(account,passwd);

}

