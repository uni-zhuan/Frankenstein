#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(900,740);   //主窗口大小设为700*750
    area = new PaintArea;
//    scrollArea = new QScrollArea;
//    scrollArea->setBackgroundRole(QPalette::Dark);   //scrollArea对象的背景色设为Dark
//    scrollArea->setWidget(area);     //将画布添加到scrollArea中
//    scrollArea->widget()->setMinimumSize(1920,1080);  //scrollArea初始化大小设为800*600
//    setCentralWidget(scrollArea);    //将scrollArea加入到主窗口的中心区
    setCentralWidget(area);
    setDockNestingEnabled(true);
    area->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(area, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slot_menu(QPoint)));
}

void MainWindow::slot_menu(QPoint){
    QMenu *menu = new QMenu;
    //自定义action
    QAction *action_delete = new QAction("delete",area);
    action_delete->setIcon(QIcon(":/icons/delete.png"));
    connect(action_delete, SIGNAL(triggered(bool)), this, SLOT(deletegra()));
    menu->addAction(action_delete);

    QAction *action_cxcy = new QAction("input cxcy",area);
    action_cxcy->setIcon(QIcon(":/icons/cxcy.png"));
    connect(action_cxcy, SIGNAL(triggered(bool)), this, SLOT(cxcyinput()));
    //添加action到menu
    menu->addAction(action_cxcy);

    QAction *action_sxsy = new QAction("input sxsy",area);
    action_sxsy->setIcon(QIcon(":/icons/sxsy.png"));
    connect(action_sxsy, SIGNAL(triggered(bool)), this, SLOT(sxsyinput()));
    //添加action到menu
    menu->addAction(action_sxsy);
    menu->move (cursor().pos());
    menu->show();
}
void MainWindow::deletegra()
{
    area->deletegra();
}

void MainWindow::cxcyinput()
{
        chooseDialog=new cxcydialog(this);
        chooseDialog->setModal(false);
        chooseDialog->show();
        connect(chooseDialog, SIGNAL(sendData_x(int)), this, SLOT(receiveData_cx(int)));
        connect(chooseDialog, SIGNAL(sendData_y(int)), this, SLOT(receiveData_cy(int)));
}

void MainWindow::sxsyinput()
{

        setDialog=new sxsydialog(this);
        setDialog->setModal(false);
        setDialog->show();
        connect(setDialog, SIGNAL(sendData_x(double)), this, SLOT(receiveData_x(double)));
        connect(setDialog, SIGNAL(sendData_y(double)), this, SLOT(receiveData_y(double)));
        connect(setDialog, SIGNAL(sendData_ok()), this, SLOT(receiveData_ok()));
        qDebug()<<"check"<<area->canScale<<area->new_sx<<area->new_sy;
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Graphics_currentIndexChanged(QString shape)
{
    if(shape == tr("None"))
        area->setShape(PaintArea::None);   //利用PaintArea类中的枚举变量
    else if(shape == tr("Line"))
        area->setShape(PaintArea::Line);
    else if(shape == tr("Circle"))
        area->setShape(PaintArea::Circle);
    else if(shape == tr("Ellipe"))
        area->setShape(PaintArea::Ellipe);
    else if(shape == tr("Eraser"))
        area->setShape(PaintArea::Eraser);
    else if(shape==tr("Polygon"))
        area->setShape(PaintArea::Polygon);
    else if(shape==tr("Bezier"))
        area->setShape(PaintArea::Bezier);
    else if(shape==tr("CurveFitting"))
        area->setShape(PaintArea::CurveFitting);
    else if(shape==tr("Bspline"))
        area->setShape(PaintArea::Bspline);
    else if(shape==tr("Triangle"))
        area->setShape(PaintArea::Triangle);
    else if(shape==tr("Rect"))
        area->setShape(PaintArea::Rect);
}

void MainWindow::on_Algorithm_currentIndexChanged(QString Algo)
{
    if(Algo == tr("DDA"))
        area->setLineAlgo(PaintArea::DDA);   //利用PaintArea类中的枚举变量
    else if(Algo == tr("Midpoint"))
        area->setLineAlgo(PaintArea::Midpoint);
    else if(Algo==tr("Bresenham"))
        area->setLineAlgo(PaintArea::Bresenham);
    else if(Algo == tr("Cohen-Sutherland"))
        area->setLineAlgo(PaintArea::CohenSutherland);
    else if(Algo==tr("MidLineClip"))
        area->setLineAlgo(PaintArea::MidLineClip);

}

void MainWindow::on_LineType_currentIndexChanged(const QString &linetype)
{
    if(linetype==tr("Straight Line"))
        area->setStraight();
    else if(linetype==tr("Dashed Line"))
        area->setDash();
    else if(linetype==tr("Dotted Line"))
        area->setDot();
}

void MainWindow::on_LineWidth_valueChanged(const int &lineWidth)
{
    area->setLineWidth(lineWidth);
}

void MainWindow::on_LineColor_clicked()
{
    QColorDialog *colorDlg = new QColorDialog(this);

    //显示颜色对话框并获取选中颜色
    //colorDlg->exec();  //模态显示，因为后面要获取选中的颜色的。
    //QColor color = colorDlg->selectedColor();  //获取当前选中的颜色

    //也可以直接写下面一行即可
    //入参是设置默认值（颜色框弹出时的默认选中颜色值）。
    QColor color = colorDlg->getColor(QColor(255,0,0));  //显示对话框并获取当前选中的颜色（显示对话框时线程阻塞，是模态显示的）

    //临时变量释放资源
    delete colorDlg;
    colorDlg = nullptr;
    area->setPenColor(color);
}

void MainWindow::on_actionClear_triggered()
{
    area->clearall();
}


void MainWindow::on_actionSelect_triggered(bool checked)
{
    area->isSelect(checked);
}


void MainWindow::on_actionFill_triggered()
{
    area->fillfigure();
}

void MainWindow::on_FixColor_clicked()
{
    QColorDialog *colorDlg = new QColorDialog(this);
    QColor color = colorDlg->getColor(QColor(255,0,0));  //显示对话框并获取当前选中的颜色（显示对话框时线程阻塞，是模态显示的）
    delete colorDlg;
    colorDlg = nullptr;
    area->setBrushColor(color);
}

void MainWindow::on_actionRotate_triggered(bool checked)
{
   area->isrotating(checked);
}

void MainWindow::on_actionLineCut_triggered()
{
    area->iscutLine();
}


void MainWindow::receiveData(int angel)
{
    area->rotatedegree=angel;     //获取传递过来的数据
    //    qDebug()<<angel;
}

void MainWindow::receiveData_x(double data)
{
    area->new_sx=data;     //获取传递过来的数据
}

void MainWindow::receiveData_y(double data)
{
    area->new_sy=data;     //获取传递过来的数据
    qDebug()<<"y"<<area->canScale<<area->new_sx<<area->new_sy;

}

void MainWindow::receiveData_cx(int data)
{
    area->cx=data;     //获取传递过来的数据
//    qDebug()<<"check"<<area->cx<<area->cy;

}

void MainWindow::receiveData_cy(int data)
{
    area->cy=data;     //获取传递过来的数据
}

void MainWindow::receiveData_ok()
{
//    qDebug()<<"ok"<<area->canScale;
    area->canScale=true;
    qDebug()<<"ok"<<area->canScale<<area->new_sx<<area->new_sy;
}

void MainWindow::receiveData_canrotate()
{

}

void MainWindow::on_checkBox_stateChanged(int arg1)
{

    area->ifchoosePoint(arg1);
//    qDebug()<<arg1;
//    if(arg1!=0)
//    {
//        chooseDialog=new cxcydialog(this);
//        chooseDialog->setModal(false);
//        chooseDialog->show();
//        connect(chooseDialog, SIGNAL(sendData_x(int)), this, SLOT(receiveData_cx(int)));
//        connect(chooseDialog, SIGNAL(sendData_y(int)), this, SLOT(receiveData_cy(int)));
//    }
}

void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    area->ifsetscale(arg1);
//    qDebug()<<arg1;
//    if(arg1!=0)
//    {
//        setDialog=new sxsydialog(this);
//        setDialog->setModal(false);
//        setDialog->show();
//        connect(setDialog, SIGNAL(sendData_x(double)), this, SLOT(receiveData_x(double)));
//        connect(setDialog, SIGNAL(sendData_y(double)), this, SLOT(receiveData_y(double)));
//        connect(setDialog, SIGNAL(sendData_ok()), this, SLOT(receiveData_ok()));
//        qDebug()<<"check"<<area->canScale<<area->new_sx<<area->new_sy;

//    }
}

void MainWindow::on_actionCurveAdjust_triggered()
{
    area->adjustCurve();
}

void MainWindow::on_Graphics_activated(const QString &shape)
{
    if(shape == tr("None"))
        area->setShape(PaintArea::None);   //利用PaintArea类中的枚举变量
    else if(shape == tr("Line"))
        area->setShape(PaintArea::Line);
    else if(shape == tr("Circle"))
        area->setShape(PaintArea::Circle);
    else if(shape == tr("Ellipe"))
        area->setShape(PaintArea::Ellipe);
    else if(shape == tr("Eraser"))
        area->setShape(PaintArea::Eraser);
    else if(shape==tr("Polygon"))
        area->setShape(PaintArea::Polygon);
    else if(shape==tr("Bezier"))
        area->setShape(PaintArea::Bezier);
    else if(shape==tr("CurveFitting"))
        area->setShape(PaintArea::CurveFitting);
    else if(shape==tr("Bspline"))
        area->setShape(PaintArea::Bspline);
    else if(shape == tr("Triangle"))
        area->setShape(PaintArea::Triangle);
    else if(shape == tr("Rect"))
        area->setShape(PaintArea::Rect);
}

void MainWindow::on_actionSave_triggered()
{
    area->saveimage();
}
