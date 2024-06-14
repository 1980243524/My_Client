#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostAddress>
#include<QString>
#include<QDebug>
#include<subthread.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    login_window=new login;
    ui->setupUi(this);
    connect(login_window,&login::open_mainwindow,this,&MainWindow::open_window);

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
