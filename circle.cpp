#include"circle.h"

circle::circle(int center_x, int center_y, int radius)
{
    xr=center_x;
    yr=center_y;
    r=radius;
}

circle::circle(int x0, int x1, int y0, int y1)
{
    xr=(x0+x1)/2;
    yr=(y0+y1)/2;
    r=abs(x0-x1)/2;

}
void circle::setcircle(int x0,int x1,int y0,int y1,int width,QColor color)
{
    figure(width,color);

    xr=(x0+x1)/2;
    yr=(y0+y1)/2;
    r=abs(x0-x1)/2;

}

void circle::draw(QPainter &p)
{
    MidPointCircle(p);
}

void circle::resetCircle(QPainter &p, int new_x, int new_y, int new_r)
{
    QPen eraser,originPen=p.pen();
    //换成橡皮擦
    eraser.setColor(Qt::white);
    p.setPen(eraser);

    //用背景色覆盖原圆
    MidPointCircle(p);

    //修改成员值
    xr=new_x;yr=new_y;
    r=new_r;

    //换回原来的笔,画出新圆
    p.setPen(originPen);
    MidPointCircle(p);
}

void circle::resetCircle(int new_x, int new_y, int new_r)
{
    //修改成员值
    xr=new_x;yr=new_y;
    r=new_r;
}

void circle::CirclePoints(QPainter &p, int x, int y, int xr, int yr,int line_i)
{
    PutPixel(p,x+xr,y+yr,line_i,  true);PutPixel(p,y+xr,x+yr,line_i, false);
    PutPixel(p,-x+xr,y+yr,line_i, true);PutPixel(p,y+xr,-x+yr,line_i, false);
    PutPixel(p,x+xr,-y+yr,line_i, true);PutPixel(p,-y+xr,x+yr,line_i, false);
    PutPixel(p,-x+xr,-y+yr,line_i, true);PutPixel(p,-y+xr,-x+yr,line_i, false);
}


void circle::MidPointCircle(QPainter &p)//(xr,yr)为圆心坐标
{
    int x,y,e;
    int line_i=0;
    x=0;y=r;e=1-r;                    // 初值e=1-r
    CirclePoints(p,x,y,xr,yr,line_i);               // 画八分对称性的其他点
    while(x<=y)                               // 画到直线x=y结束
    {
        if(e<0)
            e+=2*x+3;             // d<0，取右侧点，d增,由于优化
        else{
            e+=2*(x-y)+5; y--;      // d>=0，取右下点，d增,挑选的是y落点
        }
        x++;
        if(line_style[line_i%32])
            CirclePoints(p,x,y,xr,yr,line_i);       // 画八分对称性的其他点
        line_i++;
      }

}

bool circle::isALLin(int XL, int XR, int YB, int YT)
{
    //圆心在矩形中且圆的半径小于或等于圆心到矩形四边的距离最小值。判断多边形与窗口相交，可以采用直线方程作为判别函数来判断】
    //int x1,x2,y1,y2;
    int d,dmin;
    if(xr>=XL && xr<=XR && yr>=YB && yr<=YT)
    {
//        x1=XL;y1=YB;x2=XR;y2=YT;
//        dmin = abs((y1-y2)*xr+(x2-x1)*yr+x1*y2-y1*x2)/sqrt((y1-y2) *(y1-y2) +(x1-x2) *(x1-x2));
          //求到矩形四边的最小距离
          dmin=xr-XL;
          d=XR-xr;
          if(d<dmin) dmin=d;
          d=yr-YB;
          if(d<dmin) dmin=d;
          d=YT-yr;
          if(d<dmin) dmin=d;
          if(dmin>=r)
              return true;
          else
              return false;
    }else {
        return false;
    }
}

bool circle::isPartlyin(int XL, int XR, int YB, int YT)
{
    //矩形中心(x0, y0), 矩形上边中心(x1, y1), 矩形右边中心(x2, y2)
    float x0=(XL+XR)/2.0,y0=(YB+YT)/2.0,
          x1=x0,y1=YT,
          x2=XR,y2=y0;
    float w1 = DistanceBetweenTwoPoints(x0, y0, x2, y2);
    float h1 = DistanceBetweenTwoPoints(x0, y0, x1, y1);
    float w2 = DistanceFromPointToLine(xr, yr, x0, y0, x1, y1);
    float h2 = DistanceFromPointToLine(xr, yr, x0, y0, x2, y2);

    if (w2 > w1 + r)
        return false;
    if (h2 > h1 + r)
        return false;

    if (w2 <= w1)
        return true;
    if (h2 <= h1)
        return true;

    return (w2 - w1) * (w2 - w1) + (h2 - h1) * (h2 - h1) <= r * r;
}

QPoint circle::getGcenter()
{
    return QPoint(xr,yr);
}

int circle::getRadius()
{
    return r;
}

