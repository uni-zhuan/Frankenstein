#include"figure.h"
#include<math.h>

figure::figure(int Width,QColor color,int type)
{
    figureColor=color;
    LineType=type;
    LineWidth=Width;
    for(int i=0;i<32;i++){
        line_style[i]=1;
    }
}

void figure::PutPixel(QPainter &p,int x, int y, int line_i, bool bl)
{
    if(bl)//true:-1<k<1
    {
        if(line_style[line_i%32])
        {
            for(int i=0;i<LineWidth/2;i++)
                {
                      p.drawPoint(x,y+i);
                }
                for(int i=0;i<LineWidth-LineWidth/2;i++)
                {
                      p.drawPoint(x,y-i);
                }
        }
    }
    else
    {
        if(line_style[line_i%32])
        {
                for(int i=0;i<LineWidth/2;i++)
                {
                      p.drawPoint(x+i,y);
                }
                for(int i=0;i<LineWidth-LineWidth/2;i++)
                {
                      p.drawPoint(x-i,y);
                }
        }
    }
}

void figure::setDash()
{
    LineType=3;
    for(int i=0;i<32;i++)
    {
        if(i%16==0||i%17==0||i%18==0||i%19==0||i%20==0||i%27==0||i%28==0||i%29==0||i%30==0||i%31==0)
            line_style[i]=0;
        else
            line_style[i]=1;
    }
}
void figure::setDot()
{
    LineType=2;
    for(int i=0;i<32;i++)
    {
        if(i%8==0||i%8==1||i%8==2||i%8==3)
            line_style[i]=1;
        else
            line_style[i]=0;
    }

}

void figure::setStraight()
{
    LineType=1;
    for(int i=0;i<32;i++)
    {
        line_style[i]=1;
    }
}

void figure::setWidth(const int &lineWidth)
{
    LineWidth=lineWidth;
}


