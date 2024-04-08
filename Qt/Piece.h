#ifndef PIECE_H
#define PIECE_H

class Piece
{
public:
    Piece();
    int _row;//行
    int _col;//列
    int _id;//序号
    bool _dead;//是否存活
    bool _black;//判断阵营
    //初始化函数
    void init(int id)
    {
        struct
        {
            int row,col;
        }pos[12]=
            {
            {0,0},
            {0,1},
            {0,2},
            {0,3},
            {0,4},
            {0,5},
            {1,0},
            {1,1},
            {1,2},
            {1,3},
            {1,4},
            {1,5},
            };
        _id=id;
        _dead=false;
        _black=id<12;
        if(id<12)
        {
            _row=pos[id].row;
            _col=pos[id].col;
        }
        else
        {
            _row=5-pos[id-12].row;
            _col=5-pos[id-12].col;
        }
    }
};

#endif // PIECE_H
