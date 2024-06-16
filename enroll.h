#ifndef ENROLL_H
#define ENROLL_H

#include <QDialog>
#include<QObject>
#include <QMainWindow>
#include"subthread.h"

namespace Ui {
class Enroll;
}

class Enroll : public QDialog
{
    Q_OBJECT

public:
    explicit Enroll(QWidget *parent = nullptr);
    ~Enroll();
    QThread * enroll_thread;         //执行注册任务的线程
private:
    Ui::Enroll *ui;
public slots:
    void user_enroll();
    void tologin();
signals:
    void sigenroll(QString,QString); //触发信号使线程开始执行注册任务
    void ERROR(int const widget,int const error_flag);
    void back();
};

#endif // ENROLL_H
