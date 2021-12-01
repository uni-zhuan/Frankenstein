#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QDockWidget>
#include "paintarea.h"
#include "QColorDialog"
#include "centerdialog.h"
#include "sxsydialog.h"
#include "cxcydialog.h"
#include <QMenu>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Graphics_currentIndexChanged(QString shape);

    void on_Algorithm_currentIndexChanged(QString Algo);

    void on_LineType_currentIndexChanged(const QString &linetype);

    void on_LineWidth_valueChanged(const int &lineWidth);


    void on_LineColor_clicked();

    void on_actionClear_triggered();

    void on_actionSelect_triggered(bool checked);

    void on_actionFill_triggered();

    void on_FixColor_clicked();

    void on_actionRotate_triggered(bool checked);

    void on_actionLineCut_triggered();

    void receiveData(int data);   //接收传递过来的数据的槽
    void receiveData_x(double data);   //接收传递过来的数据的槽
    void receiveData_y(double data);   //接收传递过来的数据的槽
    void receiveData_cx(int data);   //接收传递过来的数据的槽
    void receiveData_cy(int data);   //接收传递过来的数据的槽
    void receiveData_ok();   //接收可以进行缩放的信号
    void receiveData_canrotate();   //接收可以进行缩放的信号

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_actionCurveAdjust_triggered();

    void on_Graphics_activated(const QString &arg1);
    //右键菜单响应
    void slot_menu(QPoint);
    void deletegra();
    void cxcyinput();
    void sxsyinput();

    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;
    PaintArea *area;
    centerDialog *rotateDialog;//私有成员，是弹出旋转角度对话框的指针
    cxcydialog *chooseDialog;//私有成员，是弹出中心选择对话框的指针
    sxsydialog *setDialog;//私有成员，是弹出缩放范围对话框的指针
    QScrollArea *scrollArea;
};
#endif // MAINWINDOW_H
