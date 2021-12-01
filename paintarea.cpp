#include "paintarea.h"
#include<QPainter>
#include<math.h>
#include<iostream>
//#include<String>
#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8

using namespace std;

void FloodFill4(QImage &theImage,int x,int y,QColor oldColor,QColor newColor)
{                                            //区域原来填充着旧色
   if(theImage.pixelColor(x,y) == oldColor) //检测当前点，如颜色为旧色
   {    theImage.setPixelColor(x,y,newColor);   //上新色
         FloodFill4(theImage,x,y+1,oldColor,newColor);   //检测上点
         FloodFill4(theImage,x,y-1,oldColor,newColor);    //检测下点
         FloodFill4(theImage,x-1,y,oldColor,newColor);    //检测左点
         FloodFill4(theImage,x+1,y,oldColor,newColor);   //检测右点
   }
}

typedef struct{                         //记录种子点
   int x;
   int y;
} Seed;

void PaintArea::ScanLineFill4(QImage &theImage,int x,int y,QColor oldcolor,QColor newcolor)
{

    if(oldcolor==newcolor)//如果没有选择颜色填充fill选中下 点击种子点默认颜色与背景色相同  会导致程序崩溃 故在此直接return
        {
            return ;
        }

    int xl, xr, i;
    bool spanNeedFill;
    Seed pt;
    stack<Seed> st;
    //setstackempty();           //设栈空
    pt.x =x; pt.y=y;
    st.push(pt);              //将种子点压入堆栈
    while(!st.empty())          //检查堆栈状态，空返回F，否则返回T
    {
        pt = st.top();               //取堆栈顶元素
        st.pop();
        y=pt.y;
        x=pt.x;
        while(theImage.pixelColor(x,y)==oldcolor)     //向右填充
        {
            theImage.setPixelColor(x,y,newcolor);
            x++;
        }
        xr = x-1;                                    //记下xr
        x = pt.x-1;
        while(theImage.pixelColor(x,y)==oldcolor)     //向左填充
        {
           theImage.setPixelColor(x,y,newcolor);
           x--;
        }
       xl = x+1;                                     //记下xl
       x = xl;
       y = y+1;                   //处理上面一条扫描线
       if(y>=0&&y<=1080)//圈定区域防止递归次数过多导致栈溢出
       while(x<xr)                               //在上线往右扫描
       {
           spanNeedFill=false;
           while(theImage.pixelColor(x,y)==oldcolor||x<0||x>1920)  //到需填充部分的最右点
           {
               spanNeedFill=true;
               x++;
           }
           if(spanNeedFill)                        //最右点压栈
           {
                 pt.x=x-1;pt.y=y;
                 st.push(pt);
                 spanNeedFill=false;
            }
           while(theImage.pixelColor(x,y)!=oldcolor && x<xr) x++;
       }
       //处理下面一条扫描线，代码与处理上面一条扫描线类似
       x = xl;
       y = y-2;
       if(y>=0&&y<=1080)
       while(x<xr)
       {
             spanNeedFill=false;
             while(theImage.pixelColor(x,y)==oldcolor||x<0||x>1920)  //到需填充部分的最右点
             {
                 spanNeedFill=true;
                 x++;
              }
             if(spanNeedFill)                        //最右点压栈
             {
                 pt.x=x-1;pt.y=y;
                 st.push(pt);
                 spanNeedFill=false;
              }
             while(theImage.pixelColor(x,y)!=oldcolor && x<xr) x++;
        }//End of while(i<xr)

     }

}

int PaintArea::encode(QPoint p, int XL, int XR, int YB, int YT)
{
    int c=0;
      if(p.x()<XL) c|=LEFT;         // 置位CL
      if(p.x()>XR) c|=RIGHT;      // 置位CR
      if(p.y()<YB) c|=BOTTOM;   // 置位CB
      if(p.y()>YT) c|=TOP;          // 置位CT
      return c;
}

void PaintArea::CS_LineClip(line &l, int XL, int XR, int YB, int YT)
{
    int code1,code2,code;
    int x,y,x1,x2,y1,y2;
    //获取直线端点
    QPoint p1=l.getPoint0(), p2=l.getPoint1();
    x1=p1.x();y1=p1.y();
    x2=p2.x();y2=p2.y();
    code1=encode(p1,XL,XR,YB,YT);
    code2=encode(p2,XL,XR,YB,YT);             // 端点坐标编码
    while(code1!=0 ||code2!=0)     // 直到”完全可见”
    {
        if((code1&code2) !=0)
            return;  // 排除”显然不可见”情况
        code = code1;
        if(code1==0)
            code=code2;    // 求得在窗口外的点
        //按顺序检测到端点的编码不为0，才把线段与对应的窗口边界求交。
        if((LEFT&code) !=0)                 // 线段与窗口左边延长线相交
        {
            x=XL;
            y=y1+(y2-y1)*(XL-x1)/(x2-x1);
        }
        else if((RIGHT&code) !=0)        // 线段与窗口右边延长线相交
        {
            x=XR;
            y=y1+(y2-y1)*(XR-x1)/(x2-x1);
        }
        else if((BOTTOM&code) !=0)     // 线段与窗口下边延长线相交
        {
            y=YB;
            x=x1+(x2-x1)*(YB-y1)/(y2-y1);
        }
        else if((TOP & code) !=0)         // 线段与窗口上边延长线相交
        {
            y=YT;
            x=x1+(x2-x1)*(YT-y1)/(y2-y1);
        }
        if(code==code1)
        {
            x1=x;y1=y;
            code1 =encode(QPoint(x,y),XL,XR,YB,YT);
        } //裁去P1到交点
        else
        {
            x2=x;y2=y;
            code2 =encode(QPoint(x,y),XL,XR,YB,YT);
        }                     //裁去P2到交点
     }
          l.resetline(x1,y1,x2,y2);

}

void PaintArea::Mid_LineClip(line &l, int XL, int XR, int YB, int YT)
{
    int code1,code2,code;
    int x1,x2,y1,y2,rx1,ry1,rx2,ry2;
    double x,y,d;
    //获取直线端点
    QPoint p1=l.getPoint0(), p2=l.getPoint1();
    x1=p1.x();y1=p1.y();
    x2=p2.x();y2=p2.y();

    rx2=x2;ry2=y2;

    code1=encode(p1,XL,XR,YB,YT);
    code2=encode(p2,XL,XR,YB,YT);
    if((code1&code2)!=0)//
        return ;// 排除”显然不可见”情况
    while(1)//求出原p1点的最近可见点
    {
        x=(x1+x2)/2.0;
        y=(y1+y2)/2.0;
        d=sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
        if(d<=1) //精度控制
        {
            x1=x;
            y1=y;
            break;
        }
        code=encode(QPoint(x,y),XL,XR,YB,YT);
        if((code1&code) !=0) //最近可见点在p2和pm上，pm代替p1
        {
            x1=x;
            y1=y;
            code1=encode(QPoint(x1,y1),XL,XR,YB,YT);
        }
        else //最近可见点在p1和pm上，pm代替p2
        {
            x2=x;y2=y;
        }
    }
    rx1=x1;ry1=y1; //保存求出的第一个端点结果
    x2=rx2;y2=ry2;//恢复p2初值
    while(1) //在线段与窗口有交的情况中，求出离P2最近的可见点
    {
        x=(x1+x2)/2.0;
        y=(y1+y2)/2.0;
        d=sqrt((x-x2)*(x-x2)+(y-y2)*(y-y2));
        if(d<=1)
        {
            x2=x; y2=y;break;
        }
        code=encode(QPoint(x,y),XL,XR,YB,YT);
        if((code2 & code) != 0)
        {
            x2=x; y2=y; code2=encode(QPoint(x1,y1),XL,XR,YB,YT);
        }
        else
        {
            x1=x; y1=y;
        }
    }
    l.resetline(rx1,ry1,x2,y2);
}

void PaintArea::cutLine()
{
    //QBrush original_brush=pp.brush();
        int XL=lastPoint.x()<endPoint.x()? lastPoint.x() :endPoint.x();
        int XR=lastPoint.x()+endPoint.x()-XL;
        int YB=lastPoint.y()<endPoint.y()? lastPoint.y() :endPoint.y();
        int YT=lastPoint.y()+endPoint.y()-YB;
        vector<line>::iterator it;//生成一个迭代器
        for(it=vectorLine.begin();it!=vectorLine.end();it++)
        {
            if(curAlgo == CohenSutherland)
            {
                CS_LineClip(*it,XL,XR,YB,YT);
            }
            else //if(curAlgo==MidLineClip) 默认使用终点画线
            {
                Mid_LineClip(*it,XL,XR,YB,YT);
            }
        }
    //pp.setBrush(original_brush);
}

void PaintArea::selectAll(int XL,int XR,int YB,int YT)
{
    selectFigure.clear();//清除之前的选择
    qDebug()<<"全部选择图形";
    vector<line>::iterator it1;
    for (it1=vectorLine.begin();it1!=vectorLine.end();it1++) {
        if(it1->isALLin(XL,XR,YB,YT)){
            selectFigure.push_back(&(*it1));
        }
        qDebug()<<"直线";
    }
    vector<circle>::iterator it2;
    for (it2=vectorCircle.begin();it2!=vectorCircle.end();it2++) {
        if(it2->isALLin(XL,XR,YB,YT)){
            selectFigure.push_back(&(*it2));
        }
        qDebug()<<"圆";
    }
    vector<polygon>::iterator it3;
    for (it3=vectorPolygon.begin();it3!=vectorPolygon.end();it3++) {
        if(it3->isALLin(XL,XR,YB,YT)){
            selectFigure.push_back(&(*it3));
        }
        qDebug()<<"多边形";
    }
    vector<curve>::iterator it4;
    for (it4=vectorCurve.begin();it4!=vectorCurve.end();it4++) {
        if(it4->isALLin(XL,XR,YB,YT)){
            selectFigure.push_back(&(*it4));
        }
        qDebug()<<"曲线";
    }
    vector<ellipe>::iterator ite;
    for(ite=vectorEllipe.begin();ite!=vectorEllipe.end();ite++)
    {
        if(ite->isALLin(XL,XR,YB,YT)){
            selectFigure.push_back(&(*ite));
        }
        qDebug()<<"椭圆";
    }
    vector<FillSeed>::iterator itseed;
    for(itseed=vectorSeed.begin();itseed!=vectorSeed.end();itseed++)
    {
        if(itseed->x>XL && itseed->x<XR && itseed->y>YB && itseed->y<YT){
            itseed->select=true;
            qDebug()<<"种子点";
        }
    }
}

void PaintArea::selectPartly(int XL, int XR, int YB, int YT)
{
    selectFigure.clear();//清除之前的选择
    qDebug()<<"部分选择图形";
    vector<line>::iterator it1;
    for (it1=vectorLine.begin();it1!=vectorLine.end();it1++) {
        if(it1->isPartlyin(XL,XR,YB,YT)){
            selectFigure.push_back(&(*it1));
        }
        qDebug()<<"直线";
    }
    vector<circle>::iterator it2;
    for (it2=vectorCircle.begin();it2!=vectorCircle.end();it2++) {
        if(it2->isPartlyin(XL,XR,YB,YT)){
            selectFigure.push_back(&(*it2));
        }
        qDebug()<<"圆";
    }
    vector<polygon>::iterator it3;
    for (it3=vectorPolygon.begin();it3!=vectorPolygon.end();it3++) {
        if(it3->isPartlyin(XL,XR,YB,YT)){
            selectFigure.push_back(&(*it3));
        }
        qDebug()<<"多边形";
    }
    vector<curve>::iterator it4;
    for (it4=vectorCurve.begin();it4!=vectorCurve.end();it4++) {
        if(it4->isPartlyin(XL,XR,YB,YT)){
            selectFigure.push_back(&(*it4));
        }
        qDebug()<<"曲线";
    }
    vector<ellipe>::iterator ite;
    for(ite=vectorEllipe.begin();ite!=vectorEllipe.end();ite++)
    {
        if(ite->isPartlyin(XL,XR,YB,YT)){
            selectFigure.push_back(&(*ite));
        }
        qDebug()<<"椭圆";
    }
    vector<FillSeed>::iterator itseed;
    for(itseed=vectorSeed.begin();itseed!=vectorSeed.end();itseed++)
    {
        if(itseed->x>XL && itseed->x<XR && itseed->y>YB && itseed->y<YT){
            itseed->select=true;
            qDebug()<<"种子点";
        }
    }
}

void PaintArea::selectFigures()
{
    selectRect=QRect(lastPoint.x(),lastPoint.y(),(endPoint.x()-lastPoint.x()),(endPoint.y()-lastPoint.y()));
    QPainter ps(&image);   //在theImage上绘图
    QPen pen = QPen();
    pen.setColor(QColor(65,105,225));
    pen.setStyle(Qt::DashLine);
    ps.setPen(pen);//pen递给painter
    ps.drawRect(selectRect);
    update();
    int XL=lastPoint.x()<endPoint.x()? lastPoint.x() :endPoint.x();
    int XR=lastPoint.x()+endPoint.x()-XL;
    int YB=lastPoint.y()<endPoint.y()? lastPoint.y() :endPoint.y();
    int YT=lastPoint.y()+endPoint.y()-YB;
    if(endPoint.x()>lastPoint.x())//从左往右拉框，全选
        selectAll(XL,XR,YB,YT);
    else//从右往左拉框，部分选择
        selectPartly(XL,XR,YB,YT);
    curAction=Selected;
}

PaintArea::PaintArea():
    canScale(false),modified(false),isDrawing(false),doubleclick(false),release(false),select(false)
{

    image = QImage(1920,1080,QImage::Format_RGB32);
    backColor = qRgb(255,255,255);    //画布初始化背景色使用白色
    image.fill(backColor);
    penColor = Qt::black;
    brushColor = Qt::black;
    penStyle = Qt::SolidLine;
    sx=1;sy=1;  //缩放量置个初值
    new_sx=1.0;new_sy=1.0;
    cx=0;cy=0;
    rotatedegree=0;
    curShape = None;
    curAlgo = DDA;
    figure original;
    thisfigure=&original;

    curAlgo=Bresenham;
    curAction=Paint;

    paint(image);//for test
}

void PaintArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(0,0,image);
    if(isDrawing)   //如果正在绘制特殊图形，则显示临时绘图区上的内容
    {
        painter.drawImage(0,0,tempImage);
    }

}
void PaintArea::mousePressEvent(QMouseEvent *event)
{
    lastPoint = event->pos();  //获得鼠标指针的当前坐标作为起始坐标
    switch (curAction) {
    case Paint:
    {
        if(event->button() == Qt::LeftButton &&
                (curShape == Polygon || curShape == Bezier || curShape == CurveFitting || curShape == Bspline))
        {
            isDrawing = true;
            endPoint = event->pos();
            if(!usepoints.empty()) //如果多边形点集中有点，将上一个点取出作为lastpoint
            {
                lastPoint=usepoints.back();
            }
            usepoints.push_back(endPoint);//将当前点放入点集
        }else if(event->button() == Qt::LeftButton)  //绘制其他图形
        {
            isDrawing = true;
        }
        break;
    }
    case Select:
    case Linecut:
    {
        isDrawing = true;
        break;
    }
    case ChoosePoint:
    {
        if(choosePoint==1)
        {
            cx=event->x();
            cy=event->y();
            qDebug()<<"选择了指定点："<<cx<<cy;
            drawChPoint(image);
            curAction=Selected;
        }
        break;
    }
    case Fill:
    {
        fill(event->pos());
        break;
    }
    case Adjust:
    {
        thiscurve.chooseMovePoint(event->pos());
        break;
    }
    }


}

void PaintArea::mouseMoveEvent(QMouseEvent *event)
{
    endPoint = event->pos();  //获得鼠标指针的当前坐标作为终止坐标
    int dx=endPoint.x()-lastPoint.x(),dy=endPoint.y()-lastPoint.y();
    if(event->buttons()&Qt::LeftButton)   //如果鼠标左键按着的同时移动鼠标
    {
        switch (curAction) {
        case Paint:
        {
            if(curShape == None||curShape == Eraser)   //无图形和橡皮擦情况，则直接在image上绘制
            {
                isDrawing = false;
                paint(image);
            }
            else     //绘制指定图形，则在临时绘图区tempImage上绘制
            {
                tempImage = image;    //每次绘制tempImage前用上一次image中的图片对其进行填充
                paint(tempImage);
            }
            break;
        }
        case Select:
        case Linecut:
        {
            tempImage = image;
            paint(tempImage);
            break;
        }
        case Selected:
        {
            for (int i=0;i<(int)selectFigure.size();i++)
            {
                selectFigure.at(i)->draw_t(dx,dy);
            }
            vector<FillSeed>::iterator itseed;
            for(itseed=vectorSeed.begin();itseed!=vectorSeed.end();itseed++)
            {
                itseed->x+=dx;
                itseed->y+=dy;
            }
            refresh(image);
            drawChPoint(image);
            lastPoint=endPoint;
            break;
        }
        case Adjust:
        {
            vectorCurve.pop_back();
            thiscurve.MovePoint(dx,dy);
            vectorCurve.push_back(thiscurve);
            refresh(image);
            drawGiudes(image,thiscurve.getPoints());
            lastPoint=endPoint;
            break;
        }
        case Rotate:
        {
            figure f;
            for (int i=0;i<(int)selectFigure.size();i++) {
                if(choosePoint==0)
                {
                    QPoint center=selectFigure.at(i)->getGcenter();
                    //qDebug()<<center;
                    rotatedegree=f.Cosinelaw(center,lastPoint,endPoint);
                    selectFigure.at(i)->draw_r(rotatedegree,center.x(),center.y());//做旋转
                }
                else
                {
                    rotatedegree=f.Cosinelaw(QPoint(cx,cy),lastPoint,endPoint);
                    selectFigure.at(i)->draw_r(rotatedegree,cx,cy);//做旋转
                }
            }
            lastPoint=endPoint;
            refresh(image);
            drawChPoint(image);
            break;
        }
        }
    }

}

void PaintArea::mouseReleaseEvent(QMouseEvent *event)
{
    release=true;
    endPoint = event->pos();
    int dx=endPoint.x()-lastPoint.x(),dy=endPoint.y()-lastPoint.y();
    if(event->button() == Qt::LeftButton)   //如果鼠标左键释放
    {
        switch (curAction) {
        case Paint:
        {
            isDrawing = false;
            paint(image);
            break;
        }
        case Select:
        {
            isDrawing = false;
            selectFigures();
            break;
        }
        case Linecut:
        {
            cutLine();
            isDrawing = false;
            refresh(image);
            break;
        }
        case Selected:
        {
            for (int i=0;i<(int)selectFigure.size();i++)
            {
                selectFigure.at(i)->draw_t(dx,dy);
            }
            refresh(image);
            drawChPoint(image);
            break;
        }
        case Adjust:
        {
            vectorCurve.pop_back();
            thiscurve.MovePoint(dx,dy);
            vectorCurve.push_back(thiscurve);
            thiscurve.moveIndex=-1; //归还未选中状态
            refresh(image);
            drawGiudes(image,thiscurve.getPoints());
            break;
        }
        case Rotate:
        {
            figure f;
            for (int i=0;i<(int)selectFigure.size();i++) {
                if(choosePoint==0)
                {
                    QPoint center=selectFigure.at(i)->getGcenter();
                    rotatedegree=f.Cosinelaw(center,lastPoint,endPoint);
                    selectFigure.at(i)->draw_r(rotatedegree,center.x(),center.y());//做旋转
                    qDebug()<<"旋转弧度为："<<rotatedegree;
                }
                else
                {
                    rotatedegree=(int)f.Cosinelaw(QPoint(cx,cy),lastPoint,endPoint);
                    selectFigure.at(i)->draw_r(rotatedegree,cx,cy);//做旋转
                }
            }
            refresh(image);
            drawChPoint(image);
            break;
        }
        }
    }
    release=false;
}

void PaintArea::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(curAction==Paint &&
            (curShape == Polygon || curShape == Bezier || curShape == CurveFitting || curShape == Bspline))
    {
        doubleclick=true;
    }
}

void PaintArea::wheelEvent(QWheelEvent *event)
{
    if(curAction==Selected){
        if(event->delta()>0)//滚轮上滑，远离使用者，将图形变大
        {
            sx++;
            sy++;
        }
        else//滚轮下滑，图像变小
        {
            qDebug()<<"xx*********"<<sx<<sy<<"wheel";
            sx/=2;
            sy/=2;
        }
        qDebug()<<"xx"<<sx<<sy<<"wheel";
        isDrawing=true;
        if(canScale) //如果输入了不等大缩放值
        {
            for (int i=0;i<(int)selectFigure.size();i++)
            {
                if(choosePoint==0)
                {
                    QPoint center=selectFigure.at(i)->getGcenter();
                    selectFigure.at(i)->draw_s(new_sx,new_sy,center.x(),center.y());//做缩放
                }
                else
                {
                    selectFigure.at(i)->draw_s(new_sx,new_sy,cx,cy);//做缩放
                }
                new_sx=1;new_sy=1;
            }
        }else{ //默认为等大缩放
            for (int i=0;i<(int)selectFigure.size();i++)
            {
                if(choosePoint==0)
                {
                    QPoint center=selectFigure.at(i)->getGcenter();
                    selectFigure.at(i)->draw_s(sx,sy,center.x(),center.y());//做缩放
                }
                else
                {
                    selectFigure.at(i)->draw_s(sx,sy,cx,cy);//做缩放
                }
            }
            sx=1;sy=1;
        }
        refresh(image);
        drawChPoint(image);
        isDrawing=false;
    }
}


void PaintArea::paint(QImage &theImage)
{
    QPainter pp(&theImage);   //在theImage上绘图
    QPen pen = QPen();
    pen.setColor(penColor);
    pen.setStyle(penStyle);
    pp.setPen(pen);//pen递给painter

    int x,y;
    x = lastPoint.x();
    y = lastPoint.y();

//    polygon test;
//    QPoint q1(100,100),q2(150,150),q3(150,100);
//    vector<QPoint> testq;
//    testq.push_back(q1);testq.push_back(q2);testq.push_back(q3);
//    test.setpolygon(testq);
//    //test.draw_r(100,100,100);
//    //test.draw_s(2,3,0,0);
//    test.draw(pp);
//    测试操作用的

    if(curAction==Linecut || curAction== Select)
    {
        brushColor=QColor(0,0,0,70);
        QBrush brush = QBrush(brushColor);
        pp.setBrush(brush);
        pp.drawRect(lastPoint.x(),lastPoint.y(),(endPoint.x()-lastPoint.x()),(endPoint.y()-lastPoint.y()));
    }
    else if(curAction==Selected)//对已选择的图形操作
    {
        if(release){
            if(canScale)
            {
                for (int i=0;i<(int)selectFigure.size();i++)
                {
                    if(choosePoint==0)
                    {
                        QPoint center=selectFigure.at(i)->getGcenter();
                        selectFigure.at(i)->draw_s(new_sx,new_sy,center.x(),center.y());//做缩放
                    }
                    else
                    {
                        selectFigure.at(i)->draw_s(new_sx,new_sy,cx,cy);//做缩放
                    }
                }
                refresh(image);
                new_sx=1;new_sy=1;
            }
        }
    }
    else{
        switch(curShape)
        {
        case None:           //不绘制特殊图形
            {
                pp.drawLine(lastPoint.rx(),lastPoint.ry(),endPoint.rx(),endPoint.ry());
                QLine newline(lastPoint.rx(),lastPoint.ry(),endPoint.rx(),endPoint.ry());
                noneLine.push_back(newline);
                lastPoint = endPoint;  //让终止坐标变为起始坐标
                break;
            }
        case Line:           //绘制直线
            {
                thisline.setline(lastPoint.rx(),endPoint.rx(),lastPoint.ry(),endPoint.ry());
                thisfigure = &thisline;
                switch(curAlgo)
                {
                case Midpoint:
                {
                    thisline.MidPointLine(pp);
                    break;
                }
                case Bresenham:
                {
                    thisline.LineByE(pp);
                    break;
                }
                case DDA:
                {
                    thisline.DDALine(pp);
                    break;
                }
                }
                if(release)//只有在鼠标释放时，直线确定了，再存储直线
                {
                    vectorLine.push_back(thisline);
                    stackFigure.push(&vectorLine.back());
                }
            break;
            }
        case Circle:
            {
                thiscircle.setcircle(lastPoint.rx(),endPoint.rx(),lastPoint.ry(),endPoint.ry(),penWidth,penColor);
                thisfigure = &thiscircle;
                thiscircle.MidPointCircle(pp);
                if(release) //生成新的圆
                {
                    vectorCircle.push_back(thiscircle);
                    stackFigure.push(&vectorCircle.back());
                }
                break;
            }
        case Ellipe:
            {
                thisellipe.setellipe(lastPoint.rx(),lastPoint.ry(),abs(lastPoint.rx()-endPoint.rx()),abs(lastPoint.ry()-endPoint.ry()),penWidth,penColor);
                thisfigure = &thisellipe;
                thisellipe.MidpointEllipe(pp);
                if(release) //生成新的椭圆
                {
                    vectorEllipe.push_back(thisellipe);
                    stackFigure.push(&vectorEllipe.back());
                }
                break;
            }
        case Eraser:
                {
                    QPen eraser=QPen(); //生成一个橡皮擦笔
                    eraser.setColor(Qt::white);
                    eraser.setWidth(20);
                    pp.setPen(eraser); //把笔给pp使用

                    pp.drawLine(lastPoint.rx(),lastPoint.ry(),endPoint.rx(),endPoint.ry());
                    lastPoint = endPoint;  //让终止坐标变为起始坐标
                    pp.setPen(pen); //换回笔
                    break;
                }
        case Polygon:
            //左键单击选取点，选完以后左键双击画图形
        {
            if(doubleclick)//已经双击确认生成了多边形
            {
                for (int i=0;i<(int)usepoints.size();i++) {
                }
                thispolygon.setpolygon(usepoints);
                thispolygon.draw(pp);
                //newpoly.scan(pp);

                vectorPolygon.push_back(thispolygon);
                stackFigure.push(&vectorPolygon.back()); //图形指针压栈

                usepoints.clear();
                doubleclick=false;
            }else if(usepoints.size()>1)//多边形从取第二个点开始，跟踪画出每一条边
            {
                line temp_line(lastPoint.rx(),endPoint.rx(),lastPoint.ry(),endPoint.ry());
                temp_line.LineByE(pp);
            }
            break;
        }
        case Rect:
        {
            qDebug()<<"Rect";
            break;
        }
        case Triangle:
        {
            qDebug()<<"Triangle";
            break;
        }
        case Bezier:
        case CurveFitting:
        case Bspline:
        {
            if(doubleclick)//已经双击确认生成了曲线
            {
                for (int i=0;i<(int)usepoints.size();i++) {
                }
                if(curShape==Bezier)
                    thiscurve.setCurve(usepoints,bezier);
                else if(curShape==Bspline)
                    thiscurve.setCurve(usepoints,bspline);
                else if(curShape==CurveFitting)
                    thiscurve.setCurve(usepoints,bezierFit);
                thiscurve.draw(pp);

                vectorCurve.push_back(thiscurve);
                stackFigure.push(&vectorCurve.back());

                usepoints.clear();
                doubleclick=false;
                curAction=Adjust;
            }else if(usepoints.size()>1)//从取第二个点开始，跟踪画出每一条边
            {
                QPen joint,help;//画连接点和辅助线
                joint.setCapStyle(Qt::RoundCap);joint.setColor(QColor(106,90,205));joint.setWidth(8);
                help.setWidth(3);help.setStyle(Qt::DashDotLine);help.setColor(QColor(100,149,237,120));
                pp.setPen(joint);
                pp.drawPoint(lastPoint);pp.drawPoint(endPoint);
                pp.setPen(help);
                pp.drawLine(lastPoint.rx(),lastPoint.ry(),endPoint.rx(),endPoint.ry());
                line temp_line(lastPoint.rx(),endPoint.rx(),lastPoint.ry(),endPoint.ry());
                temp_line.LineByE(pp);
                pp.setPen(pen);
            }
            break;
        }
        }
    }
    update();   //进行更新界面显示，可引起窗口重绘事件，重绘窗口
    modified = true;
}

void PaintArea::fill(QPoint seed)
{
    QPainter pp(&image);
    QPen pen = QPen();
    pen.setColor(penColor);
    pen.setStyle(penStyle);

    QColor pos_color = image.pixelColor(seed);//拾取种子点颜色
    ScanLineFill4(image,seed.x(),seed.y(),pos_color,brushColor);
    FillSeed new_seed;
    new_seed.x=seed.x();
    new_seed.y=seed.y();
    new_seed.scolor=brushColor;
    vectorSeed.push_back(new_seed);
    update();
}

void PaintArea::refresh(QImage &theImage)
{
    theImage.fill(backColor);
    update();
    QPainter pr(&theImage);
    QPen pen = QPen();
    pen.setColor(penColor);
    pen.setStyle(penStyle);
    pr.setPen(pen);
    vector<line>::iterator it;//生成一个迭代器
    for(it=vectorLine.begin();it!=vectorLine.end();it++)
    {
        it->draw(pr);
    }
    vector<circle>::iterator itc;//刷新所有的圆
    for(itc=vectorCircle.begin();itc!=vectorCircle.end();itc++)
    {
        itc->MidPointCircle(pr);
    }
    vector<polygon>::iterator itp;//刷新所有的多边形
    for(itp=vectorPolygon.begin();itp!=vectorPolygon.end();itp++)
    {
        itp->draw(pr);
    }
    vector<ellipe>::iterator ite;//刷新所有的曲线
    for(ite=vectorEllipe.begin();ite!=vectorEllipe.end();ite++)
    {
        ite->draw(pr);
    }
    vector<curve>::iterator it4;//刷新所有的曲线
    for(it4=vectorCurve.begin();it4!=vectorCurve.end();it4++)
    {
        it4->draw(pr);
    }
    vector<QLine>::iterator its = noneLine.begin();
    while (its != noneLine.end())
    {
         pr.drawLine(*its);
         ++its;
    }
    vector<FillSeed>::iterator itseed;
    for(itseed=vectorSeed.begin();itseed!=vectorSeed.end();itseed++)
    {
        ScanLineFill4(image,itseed->x,itseed->y,image.pixelColor(itseed->x,itseed->y),itseed->scolor);
    }
    update();
}

void PaintArea::drawGiudes(QImage &theImage, vector<QPoint> helpDot)
{
    QPen joint,help;//画连接点和辅助线
    QPainter ph(&theImage);
    joint.setCapStyle(Qt::RoundCap);joint.setColor(QColor(106,90,205));joint.setWidth(8);
    help.setWidth(3);help.setStyle(Qt::DashDotLine);help.setColor(QColor(100,149,237,120));
    ph.setPen(joint);
    for(int i=0;i<(int)helpDot.size();i++)
    {
        ph.drawPoint(helpDot.at(i));
    }
    ph.setPen(help);
    for(int i=1;i<(int)helpDot.size();i++)
    {
        ph.drawLine(helpDot.at(i-1),helpDot.at(i));
    }
    update();
}

void PaintArea::drawChPoint(QImage &theImage)
{
    if(choosePoint)
    {
        QPen chp;
        QPainter po(&theImage);
        chp.setCapStyle(Qt::RoundCap);chp.setColor(QColor(0,139,139));chp.setWidth(12);
        po.setPen(chp);
        po.drawPoint(cx,cy);
        update();
    }
}

void PaintArea::savemypic(QImage &theimage)
{
    theimage.save("mypic.BMP","BMP",100);
}

int PaintArea::getSy() const
{
    return sy;
}

void PaintArea::setSy(int value)
{
    sy = value;
}

void PaintArea::deletegra()
{
    qDebug()<<"delete these graphics";
    for(int i=0;i<(int)selectFigure.size();i++)
    {
        vector<line>::iterator it1;
        for (it1=vectorLine.begin();it1!=vectorLine.end();) {
            if(&(*it1)==selectFigure.at(i)){
                it1=vectorLine.erase(it1);
                qDebug()<<"直线";
                break;
            }else{
                it1++;
            }
        }
        vector<circle>::iterator it2;
        for (it2=vectorCircle.begin();it2!=vectorCircle.end();) {
            if(&(*it2)==selectFigure.at(i)){
                it2=vectorCircle.erase(it2);
                qDebug()<<"圆";
                break;
            }else{
                it2++;
            }
        }
        vector<polygon>::iterator it3;
        for (it3=vectorPolygon.begin();it3!=vectorPolygon.end();) {
            if(&(*it3)==selectFigure.at(i)){
                it3=vectorPolygon.erase(it3);
                qDebug()<<"多边形";
                break;
            }else{
                it3++;
            }
        }
        vector<curve>::iterator it4;
        for (it4=vectorCurve.begin();it4!=vectorCurve.end();) {
            if(&(*it4)==selectFigure.at(i)){
                it4=vectorCurve.erase(it4);
                qDebug()<<"曲线";
                break;
            }else{
                it4++;
            }
        }
        vector<ellipe>::iterator ite;
        for(ite=vectorEllipe.begin();ite!=vectorEllipe.end();ite++)
        {
            if(&(*ite)==selectFigure.at(i)){
                ite=vectorEllipe.erase(ite);
                qDebug()<<"椭圆";
                break;
            }else{
                ite++;
            }
        }
    }
    vector<FillSeed>::iterator itseed;
    for(itseed=vectorSeed.begin();itseed!=vectorSeed.end();)
    {
            if(itseed->select==true){
                itseed=vectorSeed.erase(itseed);
                qDebug()<<"种子点";
            }
            else{
                itseed++;
            }
    }
    refresh(image);
    qDebug()<<"delete finish";
}

void PaintArea::saveimage()
{
    savemypic(image);
}

void PaintArea::setPenColor(QColor color)
{
    penColor = color;
}

void PaintArea::setBrushColor(QColor color)
{
    brushColor = color;
}

void PaintArea::setShape(ShapeType shape)
{
    curShape = shape;
    curAction=Paint;
}
void PaintArea::setLineAlgo(AlgoType algo)
{
    curAlgo=algo;
}

void PaintArea::setAction(PaintArea::Actions act)
{
    curAction=act;
}

void PaintArea::setStraight()
{
    thisfigure->setStraight();
}

void PaintArea::setDot()
{
    thisfigure->setDot();
}

void PaintArea::setDash()
{
    thisfigure->setDash();
}
void PaintArea::setLineWidth(const int &lineWidth)
{
    thisfigure->setWidth(lineWidth);
    penWidth=lineWidth;
}
void PaintArea::clearall()
{
    image.fill(backColor);
    update();
    //释放存储数据
    vectorLine.clear();
    vectorCircle.clear();
    vectorEllipe.clear();
    vectorPolygon.clear();
    vectorCurve.clear();
    noneLine.clear();
    vectorSeed.clear();
    selectFigure.clear();
    while(!stackFigure.empty())
        stackFigure.pop();
}

void PaintArea::fillfigure()
{
    curAction=Fill;
}

void PaintArea::iscutLine()
{
    curAction=Linecut;
}

void PaintArea::isSelect(bool checked)
{
    select=checked;
    if(checked==true)
        curAction=Select;
}

void PaintArea::isrotating(bool checked)
{
    if(curAction==Selected)
    {
        if(checked==true)//在已经选择的情况下按下，变成旋转状态
            curAction=Rotate;
    }else if(curAction==Rotate){
        if(checked==false)//在旋转状态下释放，回到选择状态
            curAction=Selected;
    }
}

void PaintArea::ifchoosePoint(bool checked)
{
    choosePoint=checked;
    if(choosePoint && (curAction==Selected || curAction==Rotate))//在选择图形过后选中有效
        curAction=ChoosePoint;
}

void PaintArea::ifsetscale(bool checked)
{
    setscale=checked;
}

void PaintArea::adjustCurve()
{
    qDebug()<<"you can adjust Curve";
    curAction=Adjust;

}
