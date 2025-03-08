#include "enroll.h"
#include "ui_enroll.h"

Enroll::Enroll(QWidget *parent,Networker* worker) :
    QDialog(parent),
    m_worker(worker),
    ui(new Ui::Enroll)
{
    ui->setupUi(this);
    ui->edit_passwd->setEchoMode(QLineEdit::Password);

    enroll_thread=new QThread;


    connect(ui->button_enroll,&QPushButton::clicked,this,&Enroll::user_enroll);
    connect(this,&Enroll::sigenroll,m_worker,&Networker::Enroll);

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

    emit sigenroll(account,passwd);
}

void Enroll::tologin()
{
    this->close();
    emit back();
}
