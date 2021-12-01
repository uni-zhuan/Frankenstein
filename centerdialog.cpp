#include "centerdialog.h"
#include "ui_centerdialog.h"

centerDialog::centerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::centerDialog)
{
    ui->setupUi(this);
}

centerDialog::~centerDialog()
{
    delete ui;
}

void centerDialog::on_spinBox_valueChanged(int arg1)
{
    rotatedegree=arg1;
    emit sendData(rotatedegree);
//    qDebug()<<rotatedegree;
}

void centerDialog::on_buttonBox_accepted()
{
    emit sendData_ok();
}
