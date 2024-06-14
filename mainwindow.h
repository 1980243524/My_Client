#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTcpSocket>
#include<login.h>
#include<user.h>
#include<enroll.h>
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
    User* client;

public slots:
    void open_window(User c);
    void open_enrollwindow();
    void show_error(int const widge_flag,int const error_flag);
};

#endif // MAINWINDOW_H
