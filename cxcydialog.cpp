#include "cxcydialog.h"
#include "ui_cxcydialog.h"
#include<QDebug>
cxcydialog::cxcydialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cxcydialog)
{
    ui->setupUi(this);
}

cxcydialog::~cxcydialog()
{
    delete ui;
}

void cxcydialog::on_spinBox_x_valueChanged(int arg1)
{
    cx=arg1;
    emit sendData_x(cx);
}

void cxcydialog::on_spinBox_y_valueChanged(int arg1)
{
    cy=arg1;
    emit sendData_y(cy);
    qDebug()<<cy;
}
