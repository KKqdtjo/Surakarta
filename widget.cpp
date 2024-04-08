#include "widget.h"
#include "ui_widget.h"
#include<QPaintEvent>
#include<QPainter>
#include<QMouseEvent>
#include<QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //初始化每一个棋子
    for(int i=0;i<24;i++)
    {
        _p[i].init(i);
    }

    _selectid=-1;

    blackturn=true;

    step=0;

}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent*)
{

    QPainter painter(this);

    QPen pen;

    pen.setWidth(3);

    painter.setPen(pen);

    int d=50;

    _r=d/2;

    //画竖线
    for(int i=50;i<=300;i+=50)
    {
        painter.drawLine(QPoint(i+50,100),QPoint(i+50,350));
    }

    //画横线
    for(int i=50;i<=300;i+=50)
    {
        painter.drawLine(QPoint(100,i+50),QPoint(350,i+50));
    }

    //画外旋线
    painter.drawArc(50,50,100,100,0,270*16);
    painter.drawArc(0,0,200,200,0,270*16);
    painter.drawArc(300,50,100,100,270*16,270*16);
    painter.drawArc(250,0,200,200,270*16,270*16);
    painter.drawArc(50,300,100,100,90*16,270*16);
    painter.drawArc(0,250,200,200,90*16,270*16);
    painter.drawArc(300,300,100,100,180*16,270*16);
    painter.drawArc(250,250,200,200,180*16,270*16);

    //画棋子
    for(int i=0;i<24;i++)
    {
        drawpiece(painter,i);
    }
}

//将棋盘坐标转换为像素坐标
QPoint Widget::center(int row,int col)
{
    QPoint ret;
    ret.rx()=100+col*50;
    ret.ry()=100+row*50;
    return ret;
}

//函数的重载简化函数的运用
QPoint Widget::center(int id)
{
    return center(_p[id]._row,_p[id]._col);
}

//画棋子
void Widget::drawpiece(QPainter&painter,int id)
{
    if(_p[id]._dead)
    {
        return;
    }

    //被选中的棋子变色
    if(id==_selectid)
    {
        painter.setBrush(Qt::cyan);
        painter.drawEllipse(center(id),_r,_r);

    }
    else
    {
        if(!_p[id]._black)
        {
            painter.setBrush(Qt::white);
            painter.drawEllipse(center(id),_r,_r);
        }

        if(_p[id]._black)
        {
            painter.setBrush(Qt::black);
            painter.drawEllipse(center(id),_r,_r);
        }
    }
}

//判断是否在棋盘中;
bool Widget::getRowCol(QPoint pt,int&row,int&col)
{
    for(row=0;row<=5;row++)
    {
        for(col=0;col<=5;col++)
        {
            QPoint c=center(row,col);
            int dx=c.rx()-pt.rx();
            int dy=c.ry()-pt.ry();
            int d=dx*dx+dy*dy;
            if(d<_r*_r)
            {
                return true;
            }
        }
    }
    return false;
}

//判断两个棋子是否在同一条吃子路线上
bool Widget::judgeLoc(int moveid,int row,int col,int)
{
    int flag11=0,flag21=0,flag12=0,flag22=0;
    for(int i=0;i<24;i++){
        if(_p[moveid]._row==a1[i].x&&_p[moveid]._col==a1[i].y){
            flag11=1;
        }
        if(_p[moveid]._row==b1[i].x&&_p[moveid]._col==b1[i].y){
            flag12=1;
        }
        if(row==a1[i].x&&col==a1[i].y){
            flag21=1;
        }
        if(row==b1[i].x&&col==b1[i].y){
            flag22=1;
        }
    }
    if(flag11==1&&flag21==1)return true;
    if(flag12==1&&flag22==1)return true;
    return false;

}

//判断两个棋子在哪一个吃子路线上;
int Widget::getLine(int moveid,int row,int col,int)
{
    int flag11=0,flag21=0,flag12=0,flag22=0;
    for(int i=0;i<24;i++)
    {
        if(_p[moveid]._row==a1[i].x&&_p[moveid]._col==a1[i].y)
        {
            flag11=1;
        }
        if(_p[moveid]._row==b1[i].x&&_p[moveid]._col==b1[i].y)
        {
            flag12=1;
        }
        if(row==a1[i].x&&col==a1[i].y)
        {
            flag21=1;
        }
        if(row==b1[i].x&&col==b1[i].y)
        {
            flag22=1;
        }
    }
    if(flag11==1&&flag21==1&&flag12==1&&flag22==1)
    {
        return 3;
    }
    else if(flag11==1&&flag21==1)
    {
        return 1;
    }
    else if(flag12==1&&flag22==1)
    {
        return 2;
    }
    return 0;
}

//判断两个棋子的路线上是否有棋;
bool Widget::judgeNum(int moveid,int row,int col,int killid)
{
    int line=getLine(moveid,row,col,killid);
    if(line==0)
    {
        return false;
    }
    int flag11=0,flag12=0,flag21=0,flag22=0;
    if(line==1||line==3)
    {
        //得到移动的棋子在方向数组中的位置
        int count=0, tmp1=0,tmp2=0;
        for(int i=0;i<24;i++)
        {
            if(a1[i].x==_p[moveid]._row&&a1[i].y==_p[moveid]._col)
            {
                count++;
                if(count==1)
                {
                    tmp1=i;
                }
                if(count==2)
                {
                    tmp2=i;
                    break;
                }
            }
        }
        if(count==1)
        {
            int sign1=0,sign2=0;//当sign1=0,sign2=1满足吃子条件
            if(a1[(tmp1+1)%24].x==row&&a1[(tmp1+1)%24].y==col)
            {
                if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                {
                    flag11=1;
                }
            }
            else
            {
                for(int i=(tmp1+1)%24;a1[i].x!=row||a1[i].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++)
                    {
                        //注意要判断棋子是否存活;
                        if(_p[j]._row==a1[i].x&&_p[j]._col==a1[i].y&&j!=moveid&&j!=killid&&_p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(_p[moveid]._row==row)
                    {
                        if(a1[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(_p[moveid]._col==col)
                    {
                        if(a1[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag11=1;
                }
            }
        }
        if(count==2)
        {
            int sign1=0,sign2=0;//当sign1=0,sign2=1满足吃子条件
            if(a1[(tmp1+1)%24].x==row&&a1[(tmp1+1)%24].y==col)
            {
                if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                {
                    flag11=1;
                }
            }
            else{
                for(int i=(tmp1+1)%24;a1[i].x!=row||a1[i].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++){
                        if(_p[j]._row==a1[i].x&&_p[j]._col==a1[i].y&&j!=moveid&&j!=killid&&_p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(_p[moveid]._row==row)
                    {
                        if(a1[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(_p[moveid]._col==col)
                    {
                        if(a1[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag11=1;
                }
            }
            sign1=0,sign2=0;
            if(a1[(tmp2+1)%24].x==row&&a1[(tmp2+1)%24].y==col)
            {
                if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                {
                    flag11=1;
                }
            }
            else
            {
                for(int i=(tmp2+1)%24;a1[i].x!=row||a1[i].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++)
                    {
                        if(_p[j]._row==a1[i].x&&_p[j]._col==a1[i].y&&j!=moveid&&j!=killid&&_p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(_p[moveid]._row==row)
                    {
                        if(a1[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(_p[moveid]._col==col)
                    {
                        if(a1[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag11=1;
                }
            }
        }
        count=0, tmp1=0,tmp2=0;
        for(int i=0;i<24;i++)
        {
            if(a2[i].x==_p[moveid]._row&&a2[i].y==_p[moveid]._col)
            {
                count++;
                if(count==1)
                {
                    tmp1=i;
                }
                if(count==2)
                {
                    tmp2=i;
                    break;
                }
            }
        }
        if(count==1)
        {
            int sign1=0,sign2=0;
            if(a2[(tmp1+1)%24].x==row&&a2[(tmp1+1)%24].y==col)
            {
                if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                {
                    flag12=1;
                }
            }
            else
            {
                for(int i=(tmp1+1)%24;a2[i].x!=row||a2[i].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++)
                    {
                        if(_p[j]._row==a2[i].x&&_p[j]._col==a2[i].y&&j!=moveid&&j!=killid&&_p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(_p[moveid]._row==row)
                    {
                        if(a2[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(_p[moveid]._col==col)
                    {
                        if(a2[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag12=1;
                }
            }
        }
        if(count==2)
        {
            int sign1=0,sign2=0;
            if(a2[(tmp1+1)%24].x==row&&a2[(tmp1+1)%24].y==col)
            {
                if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                {
                    flag12=1;
                }
            }
            else
            {
                for(int i=(tmp1+1)%24;a2[i].x!=row||a2[i].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++)
                    {
                        if(_p[j]._row==a2[i].x&&_p[j]._col==a2[i].y&&j!=moveid&&j!=killid&&_p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(_p[moveid]._row==row)
                    {
                        if(a2[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(_p[moveid]._col==col)
                    {
                        if(a2[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag12=1;
                }
            }
            sign1=0,sign2=0;
            if(a2[(tmp2+1)%24].x==row&&a2[(tmp2+1)%24].y==col)
            {
                if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                {
                    flag12=1;
                }
            }
            else
            {
                for(int i=(tmp2+1)%24;a2[i].x!=row||a2[i].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++)
                    {
                        if(_p[j]._row==a2[i].x&&_p[j]._col==a2[i].y&&j!=moveid&&j!=killid&&_p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(_p[moveid]._row==row)
                    {
                        if(a2[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(_p[moveid]._col==col)
                    {
                        if(a2[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag12=1;
                }
            }

        }
        if(flag11||flag12)
        {
            return true;
        }
    }
    if(line==2||line==3)
    {
        int count=0, tmp1=0,tmp2=0;
        for(int i=0;i<24;i++)
        {
            if(b1[i].x==_p[moveid]._row&&b1[i].y==_p[moveid]._col)
            {
                count++;
                if(count==1)
                {
                    tmp1=i;
                }
                if(count==2)
                {
                    tmp2=i;
                    break;
                }
            }
        }
        if(count==1)
        {
            int sign1=0,sign2=0;
            if(b1[(tmp1+1)%24].x==row&&b1[(tmp1+1)%24].y==col)
            {
                if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                {
                    flag21=1;
                }
            }
            else
            {
                for(int i=(tmp1+1)%24;b1[i].x!=row||b1[i].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++)
                    {
                        if(_p[j]._row==b1[i].x&&_p[j]._col==b1[i].y&&j!=moveid&&j!=killid&&_p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(_p[moveid]._row==row)
                    {
                        if(b1[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(_p[moveid]._col==col)
                    {
                        if(b1[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag21=1;
                }
            }
        }
        if(count==2)
        {
            int sign1=0,sign2=0;
            if(b1[(tmp1+1)%24].x==row&&b1[(tmp1+1)%24].y==col)
            {
                if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                {
                    flag21=1;
                }
            }
            else
            {
                for(int i=(tmp1+1)%24;b1[i].x!=row||b1[i].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++)
                    {
                        if(_p[j]._row==b1[i].x&&_p[j]._col==b1[i].y&&j!=moveid&&j!=killid&&_p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(_p[moveid]._row==row)
                    {
                        if(b1[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(_p[moveid]._col==col)
                    {
                        if(b1[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag21=1;
                }
            }
            sign1=0,sign2=0;
            if(b1[(tmp2+1)%24].x==row&&b1[(tmp2+1)%24].y==col)
            {
                if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                {
                    flag21=1;
                }
            }
            else
            {
                for(int i=(tmp2+1)%24;b1[i].x!=row||b1[i].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++)
                    {
                        if(_p[j]._row==b1[i].x&&_p[j]._col==b1[i].y&&j!=moveid&&j!=killid&&_p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(_p[moveid]._row==row)
                    {
                        if(b1[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(_p[moveid]._col==col)
                    {
                        if(b1[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag21=1;
                }
            }
        }
        count=0, tmp1=0,tmp2=0;
        for(int i=0;i<24;i++)
        {
            if(b2[i].x==_p[moveid]._row&&b2[i].y==_p[moveid]._col)
            {
                count++;
                if(count==1)
                {
                    tmp1=i;
                }
                if(count==2)
                {
                    tmp2=i;
                    break;
                }
            }
        }
        if(count==1)
        {
            int sign1=0,sign2=0;
            if(b2[(tmp1+1)%24].x==row&&b2[(tmp1+1)%24].y==col)
            {
                if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                {
                    flag22=1;
                }
            }
            for(int i=(tmp1+1)%24;b2[i].x!=row||b2[i].y!=col;i++)
            {
                i%=24;
                for(int j=0;j<24;j++)
                {
                    if(_p[j]._row==b2[i].x&&_p[j]._col==b2[i].y&&j!=moveid&&j!=killid&&_p[j]._dead==false)
                    {
                        sign1=1;
                        break;
                    }
                }
                if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                {
                    sign2=1;
                }
                if(_p[moveid]._row==row)
                {
                    if(b2[i].x!=row)
                    {
                        sign2=1;
                    }
                }
                if(_p[moveid]._col==col)
                {
                    if(b2[i].y!=col)
                    {
                        sign2=1;
                    }
                }
            }
            if(sign1==0&&sign2==1)
            {
                flag22=1;
            }
        }
        if(count==2){
            int sign1=0,sign2=0;
            if(b2[(tmp1+1)%24].x==row&&b2[(tmp1+1)%24].y==col)
            {
                if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                {
                    flag22=1;
                }
            }
            else{
                for(int i=(tmp1+1)%24;b2[i].x!=row||b2[i].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++){
                        if(_p[j]._row==b2[i].x&&_p[j]._col==b2[i].y&&j!=moveid&&j!=killid&&_p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(_p[moveid]._row==row)
                    {
                        if(b2[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(_p[moveid]._col==col)
                    {
                        if(b2[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag22=1;
                }
            }
            sign1=0,sign2=0;
            if(b2[(tmp2+1)%24].x==row&&b2[(tmp2+1)%24].y==col)
            {
                if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                {
                    flag22=1;
                }
            }
            else{
                for(int i=(tmp2+1)%24;b2[i].x!=row||b2[i].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++)
                    {
                        if(_p[j]._row==b2[i].x&&_p[j]._col==b2[i].y&&j!=moveid&&j!=killid&&_p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(_p[moveid]._row!=row&&_p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(_p[moveid]._row==row)
                    {
                        if(b2[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(_p[moveid]._col==col)
                    {
                        if(b2[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag22=1;
                }
            }
        }
        if(flag21||flag22)
        {
            return true;
        }
    }
    return false;
}

bool Widget::canMove(int moveid,int row,int col,int killid)
{
    //注意row,col是killid的row,col;
    //如果移动到己方棋子，则更新点击;
    if(killid!=-1)
    {
        if(_p[moveid]._black==_p[killid]._black)
        {
            _selectid=killid;
            update();
            return false;
        }
        if(_p[moveid]._black!=_p[killid]._black)
        {

            //处于角落的棋子不可能吃子或者被吃;
            if(col==0&&row==0)return false;
            if(col==0&&row==5)return false;
            if(col==5&&row==0)return false;
            if(col==5&&row==5)return false;
            if(_p[moveid]._col==0&&_p[moveid]._row==0)return false;
            if(_p[moveid]._col==0&&_p[moveid]._row==5)return false;
            if(_p[moveid]._col==5&&_p[moveid]._row==0)return false;
            if(_p[moveid]._col==5&&_p[moveid]._row==5)return false;

            //判断两者在同一条吃子路线上
            //判断两者中间是否有棋子;
            if(judgeLoc(moveid, row,col, killid))
            {
                if(judgeNum(moveid,row,col, killid))
                {
                    return true;
                }
            }
        }
    }
    else
    {
        int dr=_p[moveid]._row-row;
        int dc=_p[moveid]._col-col;
        int d=abs(dr)*10+abs(dc);
        if(d==10||d==1||d==11)
        {
            return true;
        }
    }
    return false;

}

void Widget::mouseReleaseEvent(QMouseEvent*ev)
{
    QPoint pt=ev->pos();
    int row,col;
    bool bRet=getRowCol(pt,row,col);
    if(bRet==false)
    {
        return;
    }
    int k;
    int clickid=-1;
    for(k=0;k<24;k++)
    {
        if(_p[k]._row==row&&_p[k]._col==col&&_p[k]._dead==false)
        {
            break;
        }
    }
    if(k<24)
    {
        clickid=k;
        update();
    }
    if(_selectid==-1)
    {
        if(clickid!=-1)
        {
            if(blackturn==_p[clickid]._black)
            {
                _selectid=clickid;
                update();
            }
        }
    }
    else
    {
        if(canMove(_selectid,row,col,clickid))
        {
            step++;
            if(blackturn)
            {
                qDebug()<<"STEP"<<step<<" "<<"BLACK:"<<"("<<_p[_selectid]._row+1<<","<<_p[_selectid]._col+1<<")"<<"->("<<row+1<<","<<col+1<<")";
            }
            else
            {
                qDebug()<<"STEP"<<step<<" "<<"WHITE:"<<"("<<_p[_selectid]._row+1<<","<<_p[_selectid]._col+1<<")"<<"->("<<row+1<<","<<col+1<<")";
            }
            //移动棋子
            _p[_selectid]._row=row;
            _p[_selectid]._col=col;
            if(clickid!=-1)
            {
                _p[clickid]._dead=true;
            }
            _selectid=-1;
            blackturn=!blackturn;
            update();
        }
        if(judgeEnd()==1)
        {
            qDebug()<<"The game is over";
            qDebug()<<"The winner is WHITE";
            qDebug()<<"The total step is"<<step;
        }
        if(judgeEnd()==2)
        {
            qDebug()<<"The game is over";
            qDebug()<<"The winner is BLACK";
            qDebug()<<"The total step is"<<step;
        }
    }
}

int Widget::judgeEnd()
{
    int flag1=0,flag2=0;
    for(int i=0;i<12;i++)
    {
        if(_p[i]._dead==false)
        {
            flag1=1;
            break;
        }
    }
    for(int i=12;i<24;i++)
    {
        if(_p[i]._dead==false)
        {
            flag2=1;
            break;
        }
    }
    if(flag1==0)
    {
        return 1;
    }
    if(flag2==0)
    {
        return 2;
    }
    return 0;
}
