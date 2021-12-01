#ifndef CXCYDIALOG_H
#define CXCYDIALOG_H

#include <QDialog>

namespace Ui {
class cxcydialog;
}

class cxcydialog : public QDialog
{
    Q_OBJECT

public:
    explicit cxcydialog(QWidget *parent = nullptr);
    ~cxcydialog();

private slots:
    void on_spinBox_x_valueChanged(int arg1);

    void on_spinBox_y_valueChanged(int arg1);

private:
    Ui::cxcydialog *ui;
    int cx;
    int cy;
signals:
    void sendData_x(int);   //用来传递数据x的信号
    void sendData_y(int);   //用来传递数据y的信号
};

#endif // CXCYDIALOG_H
