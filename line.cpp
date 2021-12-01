#include"line.h"
#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8

line::line(int lastPoint_x,int endPoint_x,int lastPoint_y,int endPoint_y,lineDrawType t)
{
    x0=lastPoint_x;
    y0=lastPoint_y;
    x1=endPoint_x;
    y1=endPoint_y;
    type = t;
}

void line::setline(int lastPoint_x,int endPoint_x,int lastPoint_y,int endPoint_y)
{
    x0=lastPoint_x;
    y0=lastPoint_y;
    x1=endPoint_x;
    y1=endPoint_y;
    //figure(width,color,1);
}

void line::draw(QPainter &p)
{
    switch (type) {
        case DDA:{
             DDALine(p);
             break;
        }
        case MidPoint:{
            MidPointLine(p);
            break;
        }
        case ByE:{
            LineByE(p);
            break;
        }
    }
}

void line::DDALine(QPainter &p)
{
    type = DDA;
    float dy,dx,y,x,m;
    int line_i=0;   //线性控制位元计数器
    if (x1<x0)      //在x0<x1的情况下讨论，若不满足则交换起始点(x0,y0)(x1,y1)
    {
        int temp=x0;x0=x1;x1=temp;
            temp=y0;y0=y1;y1=temp;
    }
    if(x1==x0)  //对斜率等于无穷的情况特殊处理
    {
        if(y0<y1)//斜率大于零
            for(y=y0;y<y1;y++)
            {
                PutPixel(p, x0,y,line_i,true);//衔接自然,参数用false
                line_i++;
            }
        else//斜率小于零
            for(y=y0;y>=y1;y--)
            {
                PutPixel(p, x0,y,line_i, true);
                line_i++;
            }
    }
    dx=abs(x1-x0);//已经交换过,确定x0小
    dy=abs(y1-y0);
    m=(y1-y0)*1.0/(x1-x0);
    y=y0;
    for(x=x0;x<=x1;x++)//画点
    {
        PutPixel(p, x,y,line_i, true);
        line_i++;
        y+=m;
    }

}
void line::MidPointLine(QPainter &p)//中点算法画直线(任意斜率）
{
    type = MidPoint;
    int a,b,d1,d2,d,x,y;
    float k;
    if (x1<x0)      //在x0<x1的情况下讨论，若不满足则交换起始点(x0,y0)(x1,y1)
    {
        int temp=x0;x0=x1;x1=temp;
            temp=y0;y0=y1;y1=temp;
    }
    a=y0-y1;//隐式方程得到
    b=x1-x0;
    if (b==0)//斜率正无穷或负无穷状况,假定斜率为-100
        k=-100*a;
    else
        k=(y1-y0)*1.0/(x1-x0);//计算斜率,记得转换为浮点数
    x=x0,y=y0;
    p.drawPoint(x,y);
    int line_i=0;   //线性控制位元计数器

    //对斜率分类讨论
    if (k>=0 && k<=1)
    {
        d=a+a+b;
        d1=a+a;
        d2=a+a+b+b;
        while (x<x1)
        {
            if (d<0)
            {
                x++,y++,d+=d2;
             }
            else
            {
                x++,d+=d1;
            }
            PutPixel(p, x,y,line_i, true);
            line_i++;
        }
    }else if (k>1)
    {
        d=a+b+b;//d的初始值,增量0.5a+b,乘二,此时在x方向取中点
        d1=b+b;//d的两种可能增量,代表在中点下端和上端分别需要增加的
        d2=a+a+b+b;
        while (y<y1)
         {
            if (d<0)
            {
                y++,d+=d1;}
            else
            {
                x++,y++,d+=d2;
            }
            PutPixel(p, x,y,line_i, false);
            line_i++;
        }
    }else if (k<=0 && k>=-1)
    {
        d=a+a-b;//增量a-0.5b,乘二
        d1=a+a;
        d2=a+a-b-b;
        while (x<x1)
        {
            if (d<0)
            {
                x++,d+=d1;
            }
            else
            {
                x++,y--,d+=d2;
            }
            PutPixel(p, x,y,line_i, true);
            line_i++;
        }
    }else
    {
        d=a-b-b;//增量0.5a-b,乘二,此时在x方向取中点
        d1=a+a-b-b;
        d2=0-b-b;
        while (y>y1)
        {
            if (d<0)
            {
                x++,y--,d+=d1;
            }
            else
            {
                y--,d+=d2;
            }
            PutPixel(p, x,y,line_i, false);
            line_i++;
        }
    }
}

void line::LineByE(QPainter &p)//Breseam算法画直线(任意斜率）
{
    type=ByE;

    int x,y,dx,dy,s1,s2;
    float e;//是一个误差量,控制在上或在下,https://www.cs.helsinki.fi/group/goa/mallinnus/lines/bresenh.html
    bool isgreater1 = false;    //|k|>1?
    dx=abs(x1-x0); dy=abs(y1-y0);
    s1 = x1>x0 ? 1 : -1; s2 = y1>y0 ? 1 : -1;   //记录dx`,dy正负，以判断下一点选取方向
    if(dy>dx){      //斜率大于1，dx,dy 互换,尽量统一操作
        int temp=dx;
        dx=dy;
        dy=temp;
        isgreater1=true;
    }
    e=-dx;//只用比较大于小于零,初值设为-dx
    x=x0;y=y0;
    int line_i=0;   //线性控制位元计数器
    for(int i=0;i<=dx;i++){
//        if(line_style[line_i%32])
//            p.drawPoint(x,y);
        PutPixel(p, x,y,line_i, !isgreater1);
        line_i++;
        if(!isgreater1){    //k<1,x轴方向先移动一位,长轴肯定加一
            x+=s1;
        }else{  //k>1,y轴方向先移动一位
            y+=s2;
        }
        e=e+2*dy;//累加避免乘法和浮点数
        if(e>=0){   //e>0,取增长方向对角线上的点,短轴要加一的情况
            if(!isgreater1){    //k<1,y轴方向再移动
                y+=s2;
            }else{      //k>1,x轴方向再移动,有一个坐标的转换
                x+=s1;
            }
            e-=2*dx;//大于零时e的减量,应为1
        }
    }
}

void line::resetline(int new_x1, int new_y1, int new_x2, int new_y2)
{
    x0=new_x1;y0=new_y1;
    x1=new_x2;y1=new_y2;
}

int line::encode(int x, int y, int XL, int XR, int YB, int YT)
{
    int c=0;
      if(x<XL) c|=LEFT;         // 置位CL
      if(x>XR) c|=RIGHT;      // 置位CR
      if(y<YB) c|=BOTTOM;   // 置位CB
      if(y>YT) c|=TOP;          // 置位CT
      return c;
}

bool line::isALLin(int XL, int XR, int YB, int YT)
{
    int code0=encode(x0,y0,XL, XR, YB, YT),
        code1=encode(x1,y1,XL, XR, YB, YT);
    if((code0|code1)==0)//完全在矩形框内
        return true;
    else
        return false;

}

bool line::isPartlyin(int XL, int XR, int YB, int YT)
{
    int code0=encode(x0,y0,XL, XR, YB, YT),
        code1=encode(x1,y1,XL, XR, YB, YT);
    if((code0&code1)==0)
        return true;
    else //完全不在矩形框内
        return false;
}

void line::draw_t(int tx, int ty)
{
    Vector4d t,p1,p2;

    sethc_T(tx,ty);
    t<<x0,y0,0,1;
    p1=hc*t;
    t<<x1,y1,0,1;
    p2=hc*t;
    resetline(p1(0),p1(1),p2(0),p2(1));
}

void line::draw_r(float degree, int x, int y)//逆时针为正，顺时针为负
{
    Vector4d t,p1,p2;
    sethc_T(-x,-y,0,true);  //平移矩阵  先将指定旋转点移至原点
    sethc_R(degree);
    t<<x0,y0,0,1;
    p1=hc*hc_t*t;
    t<<x1,y1,0,1;
    p2=hc*hc_t*t;
    sethc_T(x,y,0,true);//移会原来指定点
    p1=hc_t*p1;
    p2=hc_t*p2;
    //qDebug()<<p1(0)<<p1(1)<<p2(0)<<p2(1);
    resetline(qRound(p1(0)),qRound(p1(1)),qRound(p2(0)),qRound(p2(1)));
}

void line::draw_s(double sx, double sy, int x, int y)
{
    Vector4d t,p1,p2;
    sethc_T(-x,-y,0,true);  //平移矩阵  先将指定旋转点移至原点
    sethc_S(sx,sy);
    t<<x0,y0,0,1;
    p1=hc*hc_t*t;
    t<<x1,y1,0,1;
    p2=hc*hc_t*t;
    sethc_T(x,y,0,true);//移会原来指定点
    p1=hc_t*p1;
    p2=hc_t*p2;
    resetline(p1(0),p1(1),p2(0),p2(1));
}

QPoint line::getPoint0()
{
    return QPoint(x0,y0);
}

QPoint line::getPoint1()
{
    return QPoint(x1,y1);
}

QPoint line::getGcenter()
{
    int gc_x=(x0+x1)/2,gc_y=(y0+y1)/2;
    return QPoint(gc_x,gc_y);
}
