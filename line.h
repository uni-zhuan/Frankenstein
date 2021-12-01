#ifndef LINE_H
#define LINE_H
#include <QPoint>
#include <QColor>
#include <QPainter>
#include"figure.h"

enum lineDrawType {DDA, MidPoint, ByE};

class line:public figure
{
public:
    line(int lastPoint_x=0,int endPoint_x=0,int lastPoint_y=0,int endPoint_y=0,lineDrawType t=DDA);
    void setline(int lastPoint_x,int endPoint_x,int lastPoint_y,int endPoint_y);
    void draw(QPainter &p);//按直线最初的生成方式绘制直线

    void DDALine(QPainter &p);//DDA算法画直线
    void MidPointLine(QPainter &p);//中点算法画直线
    void LineByE(QPainter &p);//Bresenham算法画直线

    void resetline(int new_x1,int new_y1,int new_x2,int new_y2);//将直线参数修改为(x1,y1) (x2,y2)

    bool isALLin(int XL, int XR, int YB, int YT);//判断直线是否完全在矩形窗口内部
    bool isPartlyin(int XL, int XR, int YB, int YT);//判断直线是否部分位于矩形窗口内部

    void draw_t(int tx,int ty);
    void draw_r(float degree,int x,int y);
    void draw_s(double sx,double sy,int x,int y);

    QPoint getPoint0(); //获取直线的第一个点
    QPoint getPoint1(); //获取直线的第二个点
    QPoint getGcenter();

private:
    int x0,y0,x1,y1;   //直线两个端点(x0,y0) (x1,y1)
    lineDrawType type; //直线是以何种算法被绘制的

protected:
    int encode(int x,int y,int XL, int XR, int YB, int YT);//区域编码函数，用于判断直线和矩形窗口之间的位置

};
#endif // LINE_H
