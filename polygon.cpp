#include "polygon.h"
#include <float.h>
int getyMax(const QPoint q,const QPoint p)//得到两点中y值较大的
{
    return q.y()>p.y()?q.y():p.y();
}

int getyMin(const QPoint q,const QPoint p)
{
    return q.y()>p.y()?p.y():q.y();
}


float getx(const QPoint q,const QPoint p)
{
    return q.y()>p.y()?p.x():q.x();
}

float getDeltax(const QPoint q,const QPoint p)
{
    if(q.y()==p.y()) return FLT_MAX;
    else return (q.x()-p.x()+0.0)/(q.y()-p.y()+0.0);
}

polygon::polygon()
{

}

polygon::~polygon()
{
    delete [] et;
}

void polygon::setpolygon(vector<QPoint> q)
{
    len=q.size();
    line l;
    for(int i=0;i<len;i++)
    {
        int j=(i+1)%len;//循环取下一个点，最后一点和第一个点相连
        edge.push_back(Edge(getyMax(q[i],q[j]),getx(q[i],q[j]),getDeltax(q[i],q[j]),getyMin(q[i],q[j])));
    }
    points=q;
}

void polygon::draw(QPainter &p)
{
    line l;
    for(int i=0;i<len;i++)
    {
        int j=(i+1)%len;//循环取下一个点，最后一点和第一个点相连
        l.setline(points[i].x(), points[j].x(),points[i].y(),points[j].y());//用传入的点集建立直线
        l.LineByE(p);
    }
}

bool compare(Edge e1,Edge e2)
{//用于sortael
  return e1.x<e2.x;    //会产生升序排序,若改为>,则变为降序
}

void polygon::sortAEL()//对ael中边重新排序,按照扫描线交点x坐标递增顺序
{
    if(ael.empty()) return ;
    ael.sort(compare);
}

bool polygon::isempty()
{
    for(int i=0;i<sizeof(et);i++)
    {
        if((et[i].e).empty())
            return true;
    }
    return false;
}
void polygon::del(int y)
{
    list<Edge>::iterator it;//生成一个迭代器
    for(it=ael.begin();it!=ael.end();)
    {
        if (it->ymax==y)
            it=ael.erase(it);   
        else
            ++it;

    }
}
void polygon::scan(QPainter &p)
{
    list<Edge>::iterator it;//生成一个迭代器
    edge.sort();//为边按照设定排序
    for(it=edge.begin();it!=edge.end();it++)
        qDebug()<<it->ymin<<"  "<<it->x<<"  "<<it->deltax<<endl;
    et=new ET[len];//开辟边分类表空间
    int j=0;
    et[0].index=edge.front().ymin;//初始化et表的动作
    (et[0].e).push_back(edge.front());

    for(it=++edge.begin();it!=edge.end();it++)
    {
        if(it->deltax==FLT_MAX)//去掉水平边
            continue;
        if(it->ymin>et[j].index)//增加到新的i类中去
        {
            j++;
            et[j].index=it->ymin;
            et[j].e.push_back(*it);//将迭代到的it插入到et[j].e尾部
//            j++;
        }
        else//增加到这个i类中边的后一链表节点
        {
//            list<Edge> et[j].e;
            et[j].e.push_back(*it);
        }
    }

    int y=et[0].index;
    int t=0;
    while(1)//执行步骤直到两表均为空
    {
        if(et[t].index==y)
        {
            if(!(et[t].e).empty())//若et中第i类非空,将其中的所有边取出插入ael中
            {
                ael.splice(ael.begin(),et[t].e);//将et[t].e中节点全部移动至ael开头处
                sortAEL();//有新边插入ael时对其重新排序,按照扫描线交点x坐标递增顺序
                t++;
            }
        }
        list<Edge>::iterator it;//生成一个迭代器
        for(it=ael.begin();it!=ael.end();)//ael中边两两配对,获取填充区间,填充
        {
            for(int i=it->x;i<next(it)->x;i++)//填充
             {
                PutPixel(p,i,y,1,true);
//                qDebug()<<i;
            }
//            qDebug()<<it->x<<"  "<<it->ymax;
            it++;
            it++;
        }
        y++;//当前扫描线纵坐标y值+1
        del(y);//将ael中满足y=ymax边删去(每条边被看作上闭下开)
        for(it=ael.begin();it!=ael.end();it++)//对ael中剩下的每一条边的x递增自身的deltax值,达到使ael中x存放的是交点x的目的
        {
            it->x+=it->deltax;
        }
        if(isempty() && ael.empty())
            break;
    }

}

void polygon::resetPolygon(vector<QPoint> new_q)
{
    //清空成员值
    edge.clear();
    ael.clear();
    et=NULL;

    //修改成员值
    points=new_q;
    len=new_q.size();
    for(int i=0;i<len;i++)
    {
        int j=(i+1)%len;//循环取下一个点，最后一点和第一个点相连
        edge.push_back(Edge(getyMax(points[i],points[j]),getx(points[i],points[j]),getDeltax(points[i],points[j]),getyMin(points[i],points[j])));
    }
}

bool polygon::isALLin(int XL, int XR, int YB, int YT)
{
    //充分条件：所有边都在矩形中
    line l;
    for(int i=0;i<len;i++)
    {
        int j=(i+1)%len;//循环取下一个点，最后一点和第一个点相连
        l.setline(points[i].x(), points[j].x(),points[i].y(),points[j].y());//用传入的点集建立直线
        if(!l.isALLin(XL, XR, YB, YT)){
            return false;
        }
    }
    return true;
}

bool polygon::isPartlyin(int XL, int XR, int YB, int YT)
{
    //至少有一条边是部分在矩形中（与矩形相交）
    line l;
    for(int i=0;i<len;i++)
    {
        int j=(i+1)%len;//循环取下一个点，最后一点和第一个点相连
        l.setline(points[i].x(), points[j].x(),points[i].y(),points[j].y());//用传入的点集建立直线
        if(l.isPartlyin(XL, XR, YB, YT)){
            return true;
        }
    }
    return false;
}

void polygon::draw_t(int x, int y)
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
    resetPolygon(po);
}

void polygon::draw_r(float degree, int x, int y)//逆时针为正，顺时针为负
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
    resetPolygon(po);
}

void polygon::draw_s(double sx, double sy, int x, int y)
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
    resetPolygon(po);
}

double polygon::Area(QPoint p0, QPoint p1, QPoint p2)
{
    double area = 0;
    area = p0.x()*p1.y() + p1.x()*p2.y() + p2.x()*p0.y() - p1.x()*p0.y() - p2.x()*p1.y() - p0.x()*p2.y();
    return area / 2;
}

QPoint polygon::getGcenter()
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

