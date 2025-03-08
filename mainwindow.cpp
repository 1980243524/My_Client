#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostAddress>
#include<QString>
#include<QDebug>
#include<subthread.h>
#include<QErrorMessage>

MainWindow::MainWindow(QWidget *parent) :

    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_destinationId(4)
{

    ui->setupUi(this);

    ui->ChatBox->setReadOnly(true); // 设置为只读
    ui->ChatBox->setStyleSheet("background-color: #f0f0f0;");

    //QVBoxLayout *layout = new QVBoxLayout(this);
    //layout->addWidget(ui->ChatBox);
    //setLayout(layout);

    qRegisterMetaType<std::vector<std::pair<QString,quint32>>*>("std::vector<std::pair<QString,quint32> >*");
    // 创建线程和 Worker
    m_workerThread = new QThread(this);
    m_worker = new Networker();

    // 将 Worker 移至子线程
    m_worker->moveToThread(m_workerThread);

    login_window=new login(nullptr,m_worker);
    enroll_window=new Enroll(nullptr,m_worker);
    m_workerThread->start();

    QMetaObject::invokeMethod(m_worker, "ConnectToServer",
                              Q_ARG(QString, "127.0.0.1"), Q_ARG(quint16, 8080));


    connect(login_window,&login::open_enrollwindow,this,[this](){
        this->enroll_window->show();
    });
    connect(this->ui->SendButton,&QPushButton::clicked,this,[this](){
        emit SignalSend(this->m_id,this->m_destinationId,this->ui->ChatLine->toPlainText());
        this->ui->ChatLine->clear();
    });

    connect(ui->Channel,&QComboBox::currentTextChanged,this,[this](QString text){
        if(text=="公屏")
            m_destinationId=4;
    });
    connect(ui->FriendList,&QListWidget::clicked,this,[this](){
        if(this->ui->Channel->currentText()=="私聊"){
            m_destinationId=(*m_friends)[this->ui->FriendList->currentRow()].second;
        }
    });
    connect(this,&MainWindow::SignalSend,m_worker,&Networker::SendMessage);
    connect(this,&MainWindow::SignalAddFriend,m_worker,&Networker::AddFriend);

    connect(m_worker,&Networker::receiveFromEcho,this,[this](QString message){
        this->ui->ChatBox->append(message);
    });

    connect(m_worker,&Networker::receiveFromPublic,this,[this](QString message){
        this->ui->ChatBox->append(message);
    });

    connect(m_worker,&Networker::receiveFromMakeFriend,this,[this](QString message){
        QMessageBox::information(this,"提示","好友添加成功");
        this->ui->FriendList->addItem(m_friends->back().first);
    });

    connect(m_worker,&Networker::receiveFromEnroll,enroll_window,[this](QString message){
        QMessageBox::information(this->enroll_window,"提示","注册成功");
        qDebug()<<message;
    });

    connect(m_worker,&Networker::receiveFromPrivate,this,[this](QString message){
        this->ui->ChatBox->append(message);
    });

    connect(ui->addFriendButton,&QPushButton::clicked,this,[this](){
        bool ok;
        QString text = QInputDialog::getText(nullptr, "输入数据", "请输入用户昵称：", QLineEdit::Normal, "", &ok);
        if (ok && !text.isEmpty()) {
            qDebug() << "用户输入的数据：" << text;
            emit SignalAddFriend(text);
        }

    });
    connect(m_worker,&Networker::successfullyLogin,this,[this](quint32 id,std::vector<std::pair<QString,quint32>>* friends,QString username){
        m_friends=friends;
        m_id=id;
        this->login_window->close();
        for(auto const &f:*m_friends){
            this->ui->FriendList->addItem(f.first);

        }
        QTableWidgetItem* item=new QTableWidgetItem(QString::number(m_id));


        this->ui->UserInfo->setItem(0,0,item);
        item = new QTableWidgetItem(username);
        this->ui->UserInfo->setItem(1,0,item);
        this->show();

    });
    // connect(login_window,&login::ERROR,this,&MainWindow::show_error);
    // connect(enroll_window,&Enroll::ERROR,this,&MainWindow::show_error);
    //connect(enroll_window,&Enroll::back,this,&MainWindow::open_loginwindow);
}


MainWindow::~MainWindow()
{
    m_workerThread->quit();      // 请求线程退出
    m_workerThread->wait();      // 等待线程结束
    delete m_worker;
    delete login_window;
    delete enroll_window;
    delete ui;
}

void MainWindow::open_window(User c)
{
    this->show();
}


void MainWindow::show_error(QString errorInfo)
{
    QMessageBox::warning(this, "错误", errorInfo);
}
