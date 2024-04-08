#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<Piece.h>

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    Piece _p[24];//棋子

    int _r;//半径

    int _selectid;//选中的棋子序号

    int step;//步数

    bool blackturn;//轮次

    void paintEvent(QPaintEvent*);//绘图事件

    void drawpiece(QPainter&painter, int id);//画棋子

    void mouseReleaseEvent(QMouseEvent*);//鼠标释放事件

    bool getRowCol(QPoint pt,int&row,int&col);//判断是否出界

    bool canMove(int moveid,int row,int col,int killid);//判断是否可以移动

    bool judgeNum(int moveid,int row,int col,int killid);//判断两个棋子中间是否有棋子

    bool judgeLoc(int moveid,int row,int col,int killid);//判断是否在同一条吃子路线

    int getLine(int moveid,int row,int col,int killid);//得到在哪条吃子路线

    int judgeEnd();//判断结束

    struct myPoint
    {
        int x;int y;
    };

    //构造四个方向数组;
    //向上数组等价于向右数组
    myPoint a1[24]={
        {1,1},
        {0,1},
        {1,0},
        {1,1},
        {1,2},
        {1,3},
        {1,4},
        {1,5},
        {0,4},
        {1,4},
        {2,4},
        {3,4},
        {4,4},
        {5,4},
        {4,5},
        {4,4},
        {4,3},
        {4,2},
        {4,1},
        {4,0},
        {5,1},
        {4,1},
        {3,1},
        {2,1}
    };
    //向左数组等价于向下数组；
    myPoint a2[24]={
        {1,1},
        {1,0},
        {0,1},
        {1,1},
        {2,1},
        {3,1},
        {4,1},
        {5,1},
        {4,0},
        {4,1},
        {4,2},
        {4,3},
        {4,4},
        {4,5},
        {5,4},
        {4,4},
        {3,4},
        {2,4},
        {1,4},
        {0,4},
        {1,5},
        {1,4},
        {1,3},
        {1,2}

    };

    //向上数组等价于向右数组；
    myPoint b1[24]={
        {2,2},
        {1,2},
        {0,2},
        {2,0},
        {2,1},
        {2,2},
        {2,3},
        {2,4},
        {2,5},
        {0,3},
        {1,3},
        {2,3},
        {3,3},
        {4,3},
        {5,3},
        {3,5},
        {3,4},
        {3,3},
        {3,2},
        {3,1},
        {3,0},
        {5,2},
        {4,2},
        {3,2}
    };

    //向左数组等价于向下数组；
    myPoint b2[24]={
        {2,2},
        {2,1},
        {2,0},
        {0,2},
        {1,2},
        {2,2},
        {3,2},
        {4,2},
        {5,2},
        {3,0},
        {3,1},
        {3,2},
        {3,3},
        {3,4},
        {3,5},
        {5,3},
        {4,3},
        {3,3},
        {2,3},
        {1,3},
        {0,3},
        {2,5},
        {2,4},
        {2,3},
        };

    QPoint center(int row,int col);

    QPoint center(int id);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
