#ifndef CIRCLE_H
#define CIRCLE_H

#include <QPoint>
#include <QColor>
#include <QPainter>
#include <math.h>
#include"figure.h"


class circle:public figure
{
public:
    circle(int center_x,int center_y,int radius);//根据圆心坐标和半径构造圆
    circle(int x0=0,int x1=0,int y0=0,int y1=0);//根据鼠标拖动位置的起始点构造圆

    void setcircle(int x0,int x1,int y0,int y1,int width=1,QColor color=Qt::black);
    void draw(QPainter &p);
    void resetCircle(QPainter &p,int new_x,int new_y,int new_r);//将圆清除，并按照新的圆心和半径位置绘制
    void resetCircle(int new_x,int new_y,int new_r);//设置新的圆心和半径位置

    void MidPointCircle(QPainter &p);//(xr,yr)为圆心相对原点的偏移量

    bool isALLin(int XL, int XR, int YB, int YT);//判断圆是否完全在矩形窗口内部
    bool isPartlyin(int XL, int XR, int YB, int YT);//判断圆是否部分位于矩形窗口内部

    QPoint getGcenter();//获取圆心坐标
    int getRadius();//获取半径值

    void draw_t(int tx,int ty)
    {
        Vector4d t,p1;

        sethc_T(tx,ty);
        t<<xr,yr,0,1;
        p1=hc*t;
        resetCircle(p1(0),p1(1),r);
    }
    void draw_r(float degree,int x,int y)//逆时针为正，顺时针为负
    {
        Vector4d t,p1;
        sethc_T(-x,-y,0,true);  //平移矩阵  先将指定旋转点移至原点
        sethc_R(degree);
        t<<xr,yr,0,1;
        p1=hc*hc_t*t;
        sethc_T(x,y,0,true);//移回原来指定点
        p1=hc_t*p1;
        resetCircle(p1(0),p1(1),r);
    }
    void draw_s(double sx,double sy,int x,int y)
        {
            if(sx!=sy)
            {
                qDebug()<<"sx与sy不等，放缩后为椭圆";
                return;
            }
            Vector4d t,p1,p2;
            sethc_T(-x,-y,0,true);  //平移矩阵  先将指定旋转点移至原点
            sethc_S(sx,sy);
            t<<xr,yr,0,1;
            p1=hc*hc_t*t;
            sethc_T(x,y,0,true);//移会原来指定点
            p1=hc_t*p1;
            resetCircle(p1(0),p1(1),sx*r);
        }

private:
    int xr,yr;//圆心
    int r;//半径
protected:
    void CirclePoints(QPainter &p, int x, int y, int xr, int yr,int line_i);
};

#endif // CIRCLE_H
