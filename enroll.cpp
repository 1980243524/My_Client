#include "enroll.h"
#include "ui_enroll.h"

Enroll::Enroll(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Enroll)
{
    ui->setupUi(this);
    ui->edit_passwd->setEchoMode(QLineEdit::Password);
}

Enroll::~Enroll()
{
    delete ui;
}

void Enroll::user_enroll()
{

}
