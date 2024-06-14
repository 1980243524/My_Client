#include "enroll.h"
#include "ui_enroll.h"

Enroll::Enroll(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Enroll)
{
    ui->setupUi(this);
}

Enroll::~Enroll()
{
    delete ui;
}
