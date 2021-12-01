#ifndef POLYGON_H
#define POLYGON_H
#include "figure.h"
#include "line.h"

typedef  struct Edge//边的数据结构
{
    int ymin;
    int ymax;//边的上端点y坐标
    float x, deltax;//x:初值为边下端点的x坐标，AEL中为当前扫描线与边的交点的X坐标;deltax 边的斜率的倒数
    Edge()
    {
        ymin=0;
        ymax=0;
        x=0.0;
        deltax=0.0;
    }
    Edge(int yMax,float X,float Deltax,int yMin)
    {
        ymax=yMax;
        x=X;
        deltax=Deltax;

        ymin=yMin;
    }
    bool operator <(Edge n2)//重载操作符为在sort中使用
    {
        if(ymin!=n2.ymin) return ymin<n2.ymin;//第一关键字升序
        if(x!=n2.x) return x<n2.x;//第二关键字升序
        return deltax<n2.deltax;//第三关键字升序
    }

    bool operator > (Edge n2)//重载操作符为在sort中使用
    {
        if(ymin!=n2.ymin) return ymin>n2.ymin;//第一关键字升序
        if(x!=n2.x) return x>n2.x;//第二关键字升序
        return deltax>n2.deltax;//第三关键字升序
    }
} Edge;

typedef  struct ET//边的分类表:按照边的下端点y对非水平边进行分类的指针数组,y=i的属于第i类,同类中依次按照x\deltax递增顺序排列
{
    int index;
    list<Edge> e;
    ET()
    {

    }
    ET(int i,list<Edge> d)
    {
        index=i;
        e=d;
    }
}ET;

class polygon:public figure
{
public:
    polygon();//空构造函数
    ~polygon();
    void setpolygon(vector<QPoint> q);//用点向量构造函数
    void draw(QPainter &p);
    void scan(QPainter &p);//扫描转换
    void resetPolygon(vector<QPoint>  new_q);

    bool isALLin(int XL, int XR, int YB, int YT);//判断多边形是否完全在矩形窗口内部
    bool isPartlyin(int XL, int XR, int YB, int YT);//判断多边形是否部分位于矩形窗口内部

    void draw_t(int x,int y);
    void draw_r(float degree,int x,int y);
    void draw_s(double sx,double sy,int x,int y);

    vector<QPoint> getPoints();//获取多边形点集
    double Area(QPoint p0, QPoint p1, QPoint p2);//计算重心辅助函数
    QPoint getGcenter();//获取多边形重心

private:
    void sort();
    void sortAEL();
    bool isempty();
    void del(int y);

    vector<QPoint> points;//多边形顶点（按绘制顺序
    int len;//多边形边数
    list<Edge> edge;   //边链表
    list<Edge> ael; // 活性边表
    ET *et;     //多边形边分类表
};

#endif // POLYGON_H
