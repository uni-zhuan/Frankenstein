#ifndef CURVE_H
#define CURVE_H

#include <QObject>
#include"figure.h"

enum CurveType{//曲线类型（小写，和area中的变量区分
    bezier,
    bspline,
    bezierFit,
};
class curve:public figure
{
public:
    curve();
    void setCurve(vector<QPoint> q,CurveType t=bezier);
    void resetCurve(vector<QPoint> q);
    double DeCasteliau(double t,double p[],int n);
    void DrawBezier(QPainter &p,vector<QPoint> crolpoints);
    void DrawBezierFitCurve(QPainter &p);//Bezier曲线拟合离散点
    void DrawB_Spline(QPainter &p,vector<QPoint> crolpoints,int K=3); //
    void DrawB_SplFitCurve(QPainter &p);
    double N(int k, int i, double u); //基函数
    double N1(int i, double u);//一次b样条曲线基函数
    double N2(int i, double u);//二次b样条曲线基函数
    double N3(int i, double u);//三次b样条曲线基函数

    bool isTogether(QPoint p,QPoint q,int dis=5);//判断两点是否靠近（默认距离5）
    void chooseMovePoint(QPoint press);//选择要移动的点
    void MovePoint(int dx,int dy);//选择要移动的点
    int moveIndex=-1;//被移动的点


    void draw(QPainter &p);

    bool isALLin(int XL, int XR, int YB, int YT);
    bool isPartlyin(int XL, int XR, int YB, int YT);

    //以下仿照多边形
    void draw_t(int x,int y);
    void draw_r(float degree,int x,int y);
    void draw_s(double sx,double sy,int x,int y);

    vector<QPoint> getPoints();//获取点集
    double Area(QPoint p0, QPoint p1, QPoint p2);//计算重心辅助函数
    QPoint getGcenter();//获取重心

private:
       vector<QPoint> points;//曲线控制点（按绘制顺序
       int len;
       CurveType type;
};

#endif // CURVE_H
