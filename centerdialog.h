#ifndef CENTERDIALOG_H
#define CENTERDIALOG_H

#include <QDialog>
#include<QDebug>
namespace Ui {
class centerDialog;
}

class centerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit centerDialog(QWidget *parent = nullptr);
    ~centerDialog();

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_buttonBox_accepted();

private:
    Ui::centerDialog *ui;
    int rotatedegree;
signals:
    void sendData(int);   //用来传递数据的信号
    void sendData_ok();
};

#endif // CENTERDIALOG_H
