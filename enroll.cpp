#include "enroll.h"
#include "ui_enroll.h"

Enroll::Enroll(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Enroll)
{
    ui->setupUi(this);
    ui->edit_passwd->setEchoMode(QLineEdit::Password);

    enroll_thread=new QThread;
    SubThread *subt= new SubThread;
    subt->moveToThread(enroll_thread);

    connect(enroll_thread,&QThread::finished,subt,&SubThread::deleteLater);
    connect(ui->button_enroll,&QPushButton::clicked,this,&Enroll::user_enroll);
    connect(this,&Enroll::sigenroll,subt,&SubThread::task_enroll);
    connect(subt,&SubThread::ERROR,this,&Enroll::ERROR);
    connect(ui->button_back_login,&QPushButton::clicked,this,&Enroll::tologin);
}

Enroll::~Enroll()
{
    delete ui;
}

void Enroll::user_enroll()
{
    QString account=ui->edit_account->text();
    QString passwd=ui->edit_passwd->text();
    enroll_thread->start();
    emit sigenroll(account,passwd);
}

void Enroll::tologin()
{
    this->close();
    emit back();
}
