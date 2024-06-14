#ifndef ENROLL_H
#define ENROLL_H

#include <QDialog>

namespace Ui {
class Enroll;
}

class Enroll : public QDialog
{
    Q_OBJECT

public:
    explicit Enroll(QWidget *parent = nullptr);
    ~Enroll();

private:
    Ui::Enroll *ui;
public slots:
    void user_enroll();
};

#endif // ENROLL_H
