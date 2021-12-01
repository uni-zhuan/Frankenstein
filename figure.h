#ifndef FIGURE_H
#define FIGURE_H
#include<QPainter>
#include<QColor>
#include<QDebug>
#include<vector>
#include<stack>
#include<list>  //使用 list 需要包含此头文件
#include<iostream>
#include<algorithm>  //使用STL中的算法需要包含此头文件
#include <Eigen/Dense>
#include<math.h>
#define PI 3.1415926

using namespace Eigen;
using std::vector;
using std::stack;
using std::list;
class figure
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    figure(int Width=3,QColor color=Qt::black,int type=1);

    int LineWidth;//图形线宽
    int line_style[32];//线型控制位串
    QColor figureColor;
    Matrix4d hc,hc_t;
    int LineType;//1为直线，2为点画线，3位虚线

    //    Matrix4d gethc();
    //    Matrix4d gethc_t();
    //    void sethc_T(int tx,int ty,int tz=0);//平移
    //    void sethc_R(int θ);//旋转
    //    void sethc_S(int sx,int sy,int sz=1);//放缩
    Matrix4d gethc()
    {
        return hc;
    }
    Matrix4d gethc_t()
    {
        return hc_t;
    }
    void sethc_T(int tx,int ty,int tz=0,bool bl=false)//平移  bl为true设置hc_t false设置hc
    {
        if(!bl)
            hc<<1,0,0,tx,
                    0,1,0,ty,
                    0,0,1,tz,
                    0,0,0,1;
        else
            hc_t<<1,0,0,tx,
                    0,1,0,ty,
                    0,0,1,tz,
                    0,0,0,1;
    }
    void sethc_R(float degree)//旋转
    {
        hc<<cos(degree),-sin(degree),0,0,
                sin(degree),cos(degree),0,0,
                0,0,1,0,
                0,0,0,1;
    }
    void sethc_S(double sx,double sy,double sz=1)//放缩
    {
        hc<<sx,0,0,0,
                0,sy,0,0,
                0,0,sz,0,
                0,0,0,1;
    }
    virtual void draw_t(int tx,int ty){//tx,ty平移量  平移与参照点无关。
        qDebug()<<tx<<ty;
    }
    virtual void draw_r(float degree,int x,int y){//degree:旋转弧度 逆时针为正，顺时针为负 x,y参照点
        qDebug()<<degree<<x<<y;
    }
    virtual void draw_s(double sx,double sy,int x,int y){//sx,sy放缩量，x,y参照点
        qDebug()<<sx<<sy<<x<<y;
    }
    virtual void draw(QPainter &p){}//图形绘制
    virtual QPoint getGcenter(){}
    void PutPixel(QPainter &p,int x,int y,int line_i,bool bl );//画图形像素点
    void setStraight();//设置图形线条为直线
    void setDot();//设置图形线条为点画线
    void setDash();//设置图形线条为虚线
    void setWidth(const int &lineWidth);//设置图形线条宽度

    // 计算两点之间的距离
    float DistanceBetweenTwoPoints(float x1, float y1, float x2, float y2)
    {
        return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    }

    // 计算点(x, y)到经过两点(x1, y1)和(x2, y2)的直线的距离
    float DistanceFromPointToLine(float x, float y, float x1, float y1, float x2, float y2)
    {
        float a = y2 - y1;
        float b = x1 - x2;
        float c = x2 * y1 - x1 * y2;
        assert(fabs(a) > 0.00001f || fabs(b) > 0.00001f);
        return fabs(a * x + b * y + c) / sqrt(a * a + b * b);
    }
    //余弦定理计算三角形ABC点A所在顶点对应的角度(返回值角度
    float Cosinelaw(QPoint A,QPoint B,QPoint C)
    {
        float a=DistanceBetweenTwoPoints(B.x(),B.y(),C.x(),C.y()),
              b=DistanceBetweenTwoPoints(A.x(),A.y(),C.x(),C.y()),
              c=DistanceBetweenTwoPoints(A.x(),A.y(),B.x(),B.y());
        float cosA=(b*b+c*c-a*a)/(2*b*c);
        float degree=acos(cosA);
        if( (B.x()-A.x())*(C.y()-A.y())<(C.x()-A.x())*(B.y()-A.y())) //顺逆时针判断
            degree=-degree;
        return degree;
    }
};

#endif // FIGURE_H
