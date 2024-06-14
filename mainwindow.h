#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTcpSocket>
#include<login.h>
#include<user.h>
namespace Ui {
class MainWindow;       //主窗口
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    login* login_window ;      //登录窗口
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    User* client;

public slots:
    void open_window(User c);

};

#endif // MAINWINDOW_H
