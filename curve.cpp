#include "curve.h"

curve::curve()
{
    len=0;
}

void curve::setCurve(vector<QPoint> q,CurveType t)
{
    len=q.size();
    points=q;
    type = t;
}

void curve::resetCurve(vector<QPoint> q)
{
    len=q.size();
    points=q;
}

double curve::DeCasteliau(double t, double p[],int n)//德卡斯特里奥函数
{
    double pp[20][20];
    for(int k=0;k<=n;k++)
    {
        pp[k][0]=p[k];
    }
    for(int r=1;r<=n;r++)
    {
        for(int i=0;i<=n-r;i++)
        {
            pp[i][r]=(1-t)*pp[i][r-1]+t*pp[i+1][r-1];
        }
    }
    return(pp[0][n]);
}

void curve::DrawBezier(QPainter &p, vector<QPoint> crolpoints)//绘制Bezier曲线
{
    int rate=500,n;
    n=crolpoints.size()-1;
    int line_i=0;

    double x,y;
    double px[20],py[20];
    for(int k=0;k<=n;k++)
    {
        px[k]=crolpoints[k].x();
        py[k]=crolpoints[k].y();
    }
    for(double t=0;t<=1;t+=0.01/rate)
    {
        x=DeCasteliau(t,px,n);
        y=DeCasteliau(t,py,n);
        PutPixel(p,(int)x,(int)y,line_i,true);
        if((int)x==0)
            qDebug()<<crolpoints.size()<<crolpoints[0]<<crolpoints[1]<<crolpoints[2]<<crolpoints[3];
        line_i++;

    }
}

void curve::DrawBezierFitCurve(QPainter &p)
{
    double a=0.25,b=0.25;
    vector<QPoint> fourcrol;//每次生成四个控制点
    QPoint A,B;
    if(len<=3){ //小于等于3，直接生成bezier曲线
        DrawBezier(p,points);
        return;
    }
    for(int i=0;i<len-1;i++)
    {
        fourcrol.push_back(points.at(i));
        if(i==0)
        {
            A.rx()=points.at(0).x()+a*(points.at(1).x()-points.at(0).x());
            A.ry()=points.at(0).y()+b*(points.at(1).y()-points.at(0).y());
        }
        else{
            A.rx()=points.at(i).x()+a*(points.at(i+1).x()-points.at(i-1).x());
            A.ry()=points.at(i).y()+b*(points.at(i+1).y()-points.at(i-1).y());
        }
        fourcrol.push_back(A);
        if(i==(len-2))
        {
            B.rx()=points.at(i+1).x()-a*(points.at(i+1).x()-points.at(i).x());
            B.ry()=points.at(i+1).y()-b*(points.at(i+1).y()-points.at(i).y());
        }
        else{
            B.rx()=points.at(i+1).x()-a*(points.at(i+2).x()-points.at(i).x());
            B.ry()=points.at(i+1).y()-b*(points.at(i+2).y()-points.at(i).y());
        }
        fourcrol.push_back(B);
        fourcrol.push_back(points.at(i+1));
        DrawBezier(p,fourcrol);
        //qDebug()<<fourcrol[0]<<fourcrol[1]<<fourcrol[2]<<fourcrol[3];
        fourcrol.clear();
    }
}

void curve::DrawB_Spline(QPainter &p, vector<QPoint> crolpoints, int K)
{
    int line_i=0, rate=5;
    // points_s.clear();
    for(double u=K; u<crolpoints.size(); u+=0.01/rate)
    {
        QPointF tmp(0,0);
        for(int i=0; i<(int)crolpoints.size();i++)
        {
            QPointF t = crolpoints[i];

            t*=N(K,i,u);

            tmp+=t;

        }
        qDebug()<<tmp;
        PutPixel(p,tmp.x(),tmp.y(),line_i,true);
        line_i++;
        // points_s.push_back(tmp);
    }
}

void curve::DrawB_SplFitCurve(QPainter &p)
{

}

double curve::N(int k, int i, double u)
{
    switch(k)
    {
    case 1:
        return N1(i,u);
    case 2:
        return N2(i,u);
    case 3:
        return N3(i,u);
    }
}

double curve::N1(int i, double u)
{
    double t = u-i;

    if(0<=t && t<1)
        return t;
    if(1<=t && t<2)
        return 2-t;
    return 0;
}

double curve::N2(int i, double u)
{
    double t = u-i;

    if(0<=t && t<1)
        return 0.5*t*t;
    if(1<=t && t<2)
        return 3*t-t*t-1.5;
    if(2<=t && t<3)
        return 0.5*pow(3-t,2);
    return 0;
}

double curve::N3(int i, double u)
{
    double t = u-i;
    double a = 1.0/6.0;

    if(0<=t && t<1)
        return a*t*t*t;
    if(1<=t && t<2)
        return a*(-3*pow(t-1,3) + 3*pow(t-1,2) + 3*(t-1) +1);
    if(2<=t && t<3)
        return a*(3*pow(t-2,3) - 6*pow(t-2,2) +4);
    if(3<=t && t<4)
        return a*pow(4-t,3);
    return 0;
}

bool curve::isTogether(QPoint p, QPoint q, int dis)
{
    if( (abs(p.x()-q.x())<dis) && (abs(p.y()-q.y())<dis) )
        return true;
    return false;
}

void curve::chooseMovePoint(QPoint press)
{
    for(int i=0;i<(int)points.size();i++)
    {
        if(isTogether(press,points.at(i))){
            moveIndex=i;
            qDebug()<<"choose curve point:"<<i;
        }
    }
}

void curve::MovePoint(int dx, int dy)
{
    if(moveIndex !=-1){ //有控制点被选择
        points.at(moveIndex).rx()+=dx;
        points.at(moveIndex).ry()+=dy;
    }
}

void curve::draw(QPainter &p)
{
    switch (type) {
    case bezier:
    {
        DrawBezier(p,points);
        break;
    }
    case bspline:
    {
        DrawB_Spline(p,points);
        break;
    }
    case bezierFit:
    {
        DrawBezierFitCurve(p);
        break;
    }
    }
}

bool curve::isALLin(int XL, int XR, int YB, int YT)//两个端点在矩形中
{
    QPoint qstart=points.at(0),qend=points.at(len-1);
    if(qstart.x()>XL && qstart.x()<XR && qstart.y()>YB && qstart.y()<YT)
        if(qend.x()>XL && qend.x()<XR && qend.y()>YB && qend.y()<YT)
            return true;
    return false;
}

bool curve::isPartlyin(int XL, int XR, int YB, int YT)//只有一个端点在矩形中
{
    QPoint qstart=points.at(0),qend=points.at(len-1);
    bool q1=(qstart.x()>XL && qstart.x()<XR && qstart.y()>YB && qstart.y()<YT),
         q2=(qend.x()>XL && qend.x()<XR && qend.y()>YB && qend.y()<YT);
    if(q1||q2)
            return true;
    return false;
}

void curve::draw_t(int x, int y)
{
    Vector4d t;
    vector<QPoint> po;
    sethc_T(x,y);
    for(int i=0;i<(int)points.size();i++)
    {
        t<<points[i].x(),points[i].y(),0,1;
        t=hc*t;
        QPoint q(t(0),t(1));
        po.push_back(q);

    }
    resetCurve(po);
}

void curve::draw_r(float degree, int x, int y)//逆时针为正，顺时针为负
{
    Vector4d t;
    Matrix4d m,n;
    sethc_T(-x,-y,0,true);  //平移矩阵  先将指定旋转点移至原点
    m=gethc_t();//储存hc_t
    sethc_T(x,y,0,true);// 移回原来位置
    sethc_R(degree);
    vector<QPoint> po;
    for(int i=0;i<(int)points.size();i++)
    {
        t<<points[i].x(),points[i].y(),0,1;
        t=hc_t*hc*m*t;
        QPoint q(qRound(t(0)),qRound(t(1)));
        po.push_back(q);

    }
    resetCurve(po);
}

void curve::draw_s(double sx, double sy, int x, int y)
{
    Vector4d t;
    Matrix4d m,n;
    sethc_T(-x,-y,0,true);  //平移矩阵  先将指定旋转点移至原点
    m=gethc_t();//储存hc_t
    sethc_T(x,y,0,true);// 移回原来位置
    sethc_S(sx,sy);
    vector<QPoint> po;
    for(int i=0;i<(int)points.size();i++)
    {
        t<<points[i].x(),points[i].y(),0,1;
        t=hc_t*hc*m*t;
        QPoint q(t(0),t(1));
        po.push_back(q);

    }
    resetCurve(po);
}

vector<QPoint> curve::getPoints()
{
    return points;
}

double curve::Area(QPoint p0, QPoint p1, QPoint p2)
{
    double area = 0;
    area = p0.x()*p1.y() + p1.x()*p2.y() + p2.x()*p0.y() - p1.x()*p0.y() - p2.x()*p1.y() - p0.x()*p2.y();
    return area / 2;
}

QPoint curve::getGcenter()
{
    QPoint p0 = points[0];
    QPoint p1 = points[1];
    QPoint p2;
    double sumarea = 0, sumx = 0, sumy = 0;

    for (int i = 2; i < len; i++)
    {
        p2 = points[i];
        double area = Area(p0, p1, p2);//求三角形的面积
        sumarea += area;
        sumx += (p0.x() + p1.x() + p2.x())*area; //求∑cx[i] * s[i]和∑cy[i] * s[i]
        sumy += (p0.y() + p1.y() + p2.y())*area;
        p1 = p2;//求总面积
    }
    int cogX = sumx / sumarea / 3,
        cogY = sumy / sumarea / 3;
    return QPoint(cogX,cogY);
}
