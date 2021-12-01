#ifndef ELLIPE_H
#define ELLIPE_H

#include <QPoint>
#include <QColor>
#include <QPainter>
#include <math.h>
#include"figure.h"

class ellipe:public figure
{
public:
    ellipe();
    ellipe(int center_x,int center_y,int ax,int by);
    void MidpointEllipe(QPainter &p);//椭圆中点绘制算法
    void setellipe(int center_x, int center_y,int ax,int by,int width,QColor color);
    void resetEllipe(QPainter &p,int new_x,int new_y,int new_ax,int new_by);//将椭圆清除，并按照新的参数绘制
    void resetEllipe(int new_x,int new_y,int new_ax,int new_by);
    void draw(QPainter &p) override;

    bool isALLin(int XL, int XR, int YB, int YT);//判断圆是否完全在矩形窗口内部
    bool isPartlyin(int XL, int XR, int YB, int YT);//判断圆是否部分位于矩形窗口内部

    QPoint getGcenter();

    void draw_t(int tx,int ty)
        {
            Vector4d t,p1;

            sethc_T(tx,ty);
            t<<xr,yr,0,1;
            p1=hc*t;
            resetEllipe(p1(0),p1(1),a,b);
        }
    void draw_r(float degree,int x,int y)//逆时针为正，顺时针为负
    {
        if(int(degree)%9!=0) return ;
        Vector4d t,p1;
        sethc_T(-x,-y,0,true);  //平移矩阵  先将指定旋转点移至原点
        sethc_R(degree);
        t<<xr,yr,0,1;
        p1=hc*hc_t*t;
        sethc_T(x,y,0,true);//移回原来指定点
        p1=hc_t*p1;
        resetEllipe(p1(0),p1(1),a,b);
    }
    void draw_s(double sx,double sy,int x,int y)
        {

            Vector4d t,p1,p2;
            sethc_T(-x,-y,0,true);  //平移矩阵  先将指定旋转点移至原点
            sethc_S(sx,sy);
            t<<xr,yr,0,1;
            p1=hc*hc_t*t;
            sethc_T(x,y,0,true);//移会原来指定点
            p1=hc_t*p1;
            resetEllipe(p1(0),p1(1),sx*a,sy*b);
        }


private:
    int xr,yr;//中心
    int a,b;//横向半径，纵向半径
protected:
    void EllipePoints(QPainter &p, int xr, int yr, int x, int y,int line_i);

};

#endif // ELLIPE_H
