#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTcpSocket>
#include<QVBoxLayout>
#include<QInputDialog>
#include"login.h"
#include"user.h"
#include"enroll.h"

#include"networker.h"
namespace Ui {
class MainWindow;       //主窗口
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    login* login_window ;      //登录窗口
    Enroll* enroll_window;
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    quint32 m_id;
    Networker *m_worker;
    QThread *m_workerThread;       // 工作线程
    quint32 m_destinationId;
    std::vector<std::pair<QString,quint32>>* m_friends;
public slots:
    void open_window(User c);
    void show_error(QString errorInfo);

signals:
    void SignalSend(quint32,quint32,QString);
    void SignalAddFriend(QString);
};

#endif // MAINWINDOW_H
