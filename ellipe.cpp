#include "ellipe.h"

ellipe::ellipe()
{

}


ellipe::ellipe(int center_x, int center_y, int ax, int by)
{
    xr=center_x;
    yr=center_y;
    a=ax;
    b=by;
}

void ellipe::MidpointEllipe(QPainter &pp)
{
    int a2=a*a;
    int b2=b*b;
    int d2;
    int d1=b2+a2*(0.25-b);
    int x=0;
    int y=b;
    int line_i=1;
    //初始化一些计算中会用的量
    EllipePoints(pp,xr,yr,x,y,line_i);
    //|k|<1，在上面的部分
    while(b2 * (x + 1) < a2 * (y - 0.5))
    {
        if(d1<0)
            d1 += b2 * (2 * x + 3);
        else
        {
            d1 += b2 * (2 * x + 3) + a2 * (2 - 2 * y);
            y--;
        }
        x++;
        EllipePoints(pp,xr,yr,x,y,line_i);
    }
    //|k|>1，在下面的部分
    d2 = sqrt(b * (x + 0.5) + a * (y - 1)) - a * b;
    while(y>0)
    {
        if(d2<0)
        {
            d2 += b2 * (2 * x + 2) + a2 * (3 - 2 * y);
            x++;
        }
        else
        {
            d2 += a2 * (3 - 2 * y);
        }
        y--;
        EllipePoints(pp,xr,yr,x,y,line_i);
    }
}

void ellipe::setellipe(int center_x, int center_y, int ax, int by, int width, QColor color)
{
    figure(width,color);

    xr=center_x;
    yr=center_y;
    a=ax;
    b=by;
}

void ellipe::resetEllipe(QPainter &p, int new_x, int new_y, int new_ax, int new_by)
{
    //修改成员值
    xr=new_x;yr=new_y;
    a=new_ax;b=new_by;
}

void ellipe::resetEllipe(int new_x, int new_y, int new_ax, int new_by)
{
    //修改成员值
    xr=new_x;yr=new_y;
    a=new_ax;b=new_by;
}

void ellipe::draw(QPainter &p)
{
    MidpointEllipe(p);
}

bool ellipe::isALLin(int XL, int XR, int YB, int YT)
{
    //中心在矩形中且a<矩形宽,b<矩形高。
    if(xr-a>XL && xr+a<XR && yr-b>YB && yr+b<YT)
        return true;
    return false;
}

bool ellipe::isPartlyin(int XL, int XR, int YB, int YT)
{
    //待定
    //矩形中心(x0, y0), 矩形上边中心(x1, y1), 矩形右边中心(x2, y2)
    float x0=(XL+XR)/2.0,y0=(YB+YT)/2.0,
          x1=x0,y1=YT,
          x2=XR,y2=y0;
    float w1 = DistanceBetweenTwoPoints(x0, y0, x2, y2);
    float h1 = DistanceBetweenTwoPoints(x0, y0, x1, y1);
    float w2 = DistanceFromPointToLine(xr, yr, x0, y0, x1, y1);
    float h2 = DistanceFromPointToLine(xr, yr, x0, y0, x2, y2);

    if (w2 > w1 + a)
        return false;
    if (h2 > h1 + b)
        return false;

    if (w2 <= w1)
        return true;
    if (h2 <= h1)
        return true;
return (w2 - w1) * (w2 - w1) + (h2 - h1) * (h2 - h1) <= (a * a+b*b)/2*a*b;
}

QPoint ellipe::getGcenter()
{
    return QPoint(xr,yr);
}

void ellipe::EllipePoints(QPainter &p, int xr, int yr, int x, int y, int line_i)
{
    PutPixel(p,x+xr,y+yr,line_i,  true);PutPixel(p,x+xr,-y+yr,line_i, false);
    PutPixel(p,-x+xr,y+yr,line_i, true);PutPixel(p,-x+xr,-y+yr,line_i, false);

}

