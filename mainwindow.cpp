#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostAddress>
#include<QString>
#include<QDebug>
#include<subthread.h>
#include<QErrorMessage>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    login_window=new login;
    enroll_window=new Enroll;
    ui->setupUi(this);
    connect(login_window,&login::open_mainwindow,this,&MainWindow::open_window);
    connect(login_window,&login::open_enrollwindow,this,&MainWindow::open_enrollwindow);
    connect(login_window,&login::ERROR,this,&MainWindow::show_error);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open_window(User c)
{
    client=&c;
    this->show();
}

void MainWindow::open_enrollwindow()
{
    enroll_window->show();
}
void MainWindow::show_error(int const widge_flag,int const error_flag)
{
    QErrorMessage *error=nullptr;

    switch(widge_flag)
    {
    case LOGIN_FLAG:
        error=new QErrorMessage(this->login_window);
        break;
    case ENROLL_FLAG:
        error=new QErrorMessage(this->enroll_window);
        break;
    default:
        break;
    }

    switch(error_flag)
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
