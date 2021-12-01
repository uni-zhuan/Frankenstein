#ifndef SXSYDIALOG_H
#define SXSYDIALOG_H

#include <QDialog>

namespace Ui {
class sxsydialog;
}

class sxsydialog : public QDialog
{
    Q_OBJECT

public:
    explicit sxsydialog(QWidget *parent = nullptr);
    ~sxsydialog();

private slots:
    void on_doubleSpinBox_x_valueChanged(double arg1);

    void on_doubleSpinBox_y_valueChanged(double arg1);

    void on_buttonBox_accepted();

private:
    Ui::sxsydialog *ui;
    double sx;
    double sy;
signals:
    void sendData_x(double);   //用来传递数据x的信号
    void sendData_y(double);   //用来传递数据y的信号
    void sendData_ok();//用来传递可以进行缩放的信号
};

#endif // SXSYDIALOG_H
