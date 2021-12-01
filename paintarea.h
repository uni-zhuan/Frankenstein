#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPoint>
#include<QColor>
#include"line.h"
#include"circle.h"
#include"polygon.h"
#include"ellipe.h"
#include"curve.h"

class PaintArea :public QWidget
{
public:
    enum Actions
    {
        Paint,
        Save,
        Select,
        Selected,
        Fill,
        Linecut,
        Adjust,
        Rotate,
        ChoosePoint,
    };
    enum ShapeType   //枚举变量，几个图形的选择
    {
        None,
        Line,
        Circle,
        Ellipe,
        Eraser,
        Rect,
        Triangle,
        Polygon,
        Bezier,
        CurveFitting,
        Bspline,
    };
    enum AlgoType   //枚举变量，几个图形的选择
    {
        DDA,
        Midpoint,
        Bresenham,
        CohenSutherland,
        MidLineClip,
    };
    enum LineType
    {
        Straight_Line,
        Dotted_Line,
        Dashed_Line,
    };
    PaintArea();
    void setPenStyle(Qt::PenStyle style); //设置画笔风格

    void setPenWidth(int width);   //设置画笔宽度
    void setPenColor(QColor color);   //设置画笔颜色
    void setBrushColor(QColor color);   //设置填充颜色

    void setLineType(LineType linetype);
    void setShape(ShapeType shape);  //设置要绘制的图形
    void setLineAlgo(AlgoType algo);//设置直线图形算法
    void setAction(Actions act); //设置当前动作

    bool isModified() const { return modified; }  //判断画布内容是否被更改过
    void clearall();
    void fillfigure();
    void iscutLine();
    void isSelect(bool checked);
    void isrotating(bool checked);
    void ifchoosePoint(bool checked);
    void ifsetscale(bool checked);
    void adjustCurve();//调整曲线

    void setStraight();
    void setDot();
    void setDash();
    void setLineWidth(const int &lineWidth);

    void ScanLineFill4(QImage &theImage,int x,int y,QColor oldcolor,QColor newcolor);//种子点扫描线填充
    int encode(QPoint p,int XL,int XR,int YB,int YT);//计算p点的编码（XL,XR,YB,YT）定义裁剪窗口边界
    void CS_LineClip(line &l,int XL,int XR,int YB,int YT);//Cohen-Sutherland算法裁剪直线
    void Mid_LineClip(line &l,int XL,int XR,int YB,int YT);//中点分隔法裁剪直线
    void cutLine();//直线裁剪控制函数
    void selectAll(int XL,int XR,int YB,int YT);//选择全部在框中的图形放入selectFigure
    void selectPartly(int XL,int XR,int YB,int YT);//选择部分在框中的图形放入selectFigure
    void selectFigures();

    int getSy() const;
    void setSy(int value);
    //右键菜单响应
    void deletegra();
    void saveimage();

    float rotatedegree;//旋转的角度量
    bool canScale;//标志缩放开始
    double new_sx;double new_sy;//放缩量
    int cx;int cy;//指定参考点
protected:
    void paintEvent(QPaintEvent *);   //重绘事件
    void mousePressEvent(QMouseEvent *);  //鼠标按下事件
    void mouseMoveEvent(QMouseEvent *);   //鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *);   //鼠标释放事件
    void mouseDoubleClickEvent(QMouseEvent *); //鼠标双击事件（绘制多边形用）
    void wheelEvent(QWheelEvent *event);//鼠标滚轮事件（图形缩放使用）
    void paint(QImage &theImage);
    void fill(QPoint seed);
    void refresh(QImage &theImage);//刷新所有图形
    void drawGiudes(QImage &theImage,vector<QPoint> helpDot);//画出辅助线
    void drawChPoint(QImage &theImage);
    void savemypic(QImage &theimage);

private:
    QImage image;
    QRgb backColor;
    QPoint lastPoint,endPoint; //定义两个坐标对象存放鼠标指针的前后两个坐标

    QColor penColor;    //画笔颜色
    QColor brushColor;   //填充颜色
    Qt::PenStyle penStyle;    //画笔风格
    int penWidth; //线宽
    QRect selectRect;//选择框

    ShapeType curShape;    //当前图形
    AlgoType curAlgo;
    Actions curAction;

    //当前图形对象
    figure * thisfigure;
    line thisline;
    circle thiscircle; //圆对象
    ellipe thisellipe;//椭圆对象
    polygon thispolygon;
    curve thiscurve;
    //存储区
    vector<line> vectorLine;
    vector<circle> vectorCircle;
    vector<ellipe> vectorEllipe;
    vector<polygon> vectorPolygon;
    vector<curve> vectorCurve;
    stack <figure *>stackFigure;
    vector<QLine> noneLine;
    typedef struct{                         //记录种子点
       int x;
       int y;
       QColor scolor;
       bool select;
    } FillSeed;
    vector<FillSeed> vectorSeed;

    vector<figure *> selectFigure;  //存放当前选中图形
    bool modified;

    vector<QPoint> usepoints;//多边形、画曲线的点集

    //解决重影问题
    QImage tempImage;   //临时绘图区

    bool isDrawing;     //是否实时绘制图形
    bool doubleclick;   //双击
    bool release;   //是否鼠标释放
    bool select; //select标签点中标志量
    bool choosePoint; //是否指定参考点
    bool setscale;//是否使用sx=sy作为缩放比例，不是则另外自己输
    double sx;double sy;//放缩量
};

#endif // PAINTAREA_H
