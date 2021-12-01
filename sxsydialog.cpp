#include "sxsydialog.h"
#include "ui_sxsydialog.h"

sxsydialog::sxsydialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sxsydialog)
{
    ui->setupUi(this);
}

sxsydialog::~sxsydialog()
{
    delete ui;
}


void sxsydialog::on_doubleSpinBox_x_valueChanged(double arg1)
{
    sx=arg1;
    emit sendData_x(sx);
}

void sxsydialog::on_doubleSpinBox_y_valueChanged(double arg1)
{
    sy=arg1;
    emit sendData_y(sy);
}


void sxsydialog::on_buttonBox_accepted()
{
    emit sendData_ok();
}
