//
// Created by litm on 2020/2/4.
//

#ifndef JUMP0_CHESSINCLUDE_H
#define JUMP0_CHESSINCLUDE_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <list>
#include <map>
#include <opencv2/core/core.hpp>



using namespace cv;
using namespace std;
//#include "ChinessJumpChessControl.h"
#define WINDOW_NAME_CHESS "CHESS JUMP CHESS"
const int CHESS_RADIUS = 19;//棋子的圆的直径
const int DIAMETER_BIG_CIRCLE = 290;  //棋盘外围大圆的直径
const int  DIAMETER_MIN_CIRCLE = 9;    //跳棋的每个小圆的直径
const int DISTANCE_MIN_CIRCLE_2_BIG_CIRCLE = 16; //三角性顶点的小圆的距离外围大圆的距离，小圆外围顶点到大圆边缘
const int  DISTANCE_MIN_CIRCLE_2_MIN_CIRCLE = 9;  //小圆和小圆之间的边缘之间的距离
const int SHARELIGHTGREEN_COUNT=61;
//定义棋子的颜色，6个角，有6种颜色
enum ChessColor{ RED=1, BLUE=2, WHITE=3, GREEN=4,ORANGE=5,ROSERED=6,SHARELIGHTGREEN=0,NOCHESS=-1};

enum NextType{SELF=0,TLEFT=1,TRIGHT=2,MLEFT=3,MRIGHT=4,DLEFT=5,DRIGHT=6};
//定义空位置的颜色，除了6个角和棋子颜色一样外，中间还有一块浅绿色的共享区域的颜色
//enum ChessCircleColor{ ,SHARELIGHTGREEN};
const int MAX_X=17;
const int MAX_Y=17;
//跳棋的棋子
class CheckersMapLimitCheck_include_version {
public:
    int  mapXCountToYMin2Max_2[MAX_X][2] {
            {5,5},//X=1, Y:Min=5:Max=5
            {5,6},//X=2, Y:Min=5:Max=6
            {5,7},//X=3, Y:Min=5:Max=7
            {5,8},//X=4, Y:Min=5:Max=8
            {1,13},//X=5,Y:Min=1:Max=13
            {2,13},//X=6,Y:Min=2,Max=13
            {3,13},//X=7,Y:Min=3:Max=13
            {4,13},//X=8,Y:Min=4:Max=13
            {5,13},//X=9,Y:Min=5:Max=13
            {5,14},//X=10,Y:Min=5:Max=14
            {5,15},//X=11,y:Min=5:Max=15
            {5,16},//X=12,Y:Min=5,Max=16
            {5,17},//X=13,Y:Min=5,Max=17
            {10,13},//X=14,Y:Min=10,Max=13
            {11,13},//X=15,Y:Min=11,Max=13
            {12,13},//X=16,Y:Min=11,Max=13
            {13,13},//X=17,Y:Min=13,Max=13
    };
    bool IsLegalPosition(int x ,int y) {
        if(x<1 || x>MAX_X|| (y <1 || y >MAX_Y)){
            return false;
        }


        if((y< mapXCountToYMin2Max_2[x-1][0])||
           (y> mapXCountToYMin2Max_2[x-1][1])){
            return false;
        }
        return true;
    }
};
static CheckersMapLimitCheck_include_version chesschecker_inner;
class Chess{
private:
    enum ChessColor color;//当前棋子的颜色
    int x;//我在棋盘的位置
    int y;
public:
    Chess(){
        color = RED;
        x = 0;
        y = 0;
    }
    Chess( ChessColor color,int x,int y){
        if(chesschecker_inner.IsLegalPosition(x,y) == false)
            return;
        this->color = color;
        this->x  = x;
        this->y  = y;
    }
    int getx();
    int gety();
    ChessColor getcolor();
    void setpos(int x,int y);
    void setcolor( ChessColor color);
};
//每个跳棋的空位
class ChessCircle{
private:
    bool dumpOrchessFlag;//是否这个空位置有棋子的标注
    ChessColor color;//当前空位置的颜色
    ChessCircle *nextChessCircle[6];//最多是6个连接，最少2个，没有连接的是空

    int x;//空位置在棋盘上的位置
    int y;
public:
    ChessCircle(){
        dumpOrchessFlag = false;
        color = SHARELIGHTGREEN;
        for(int i=0 ; i < 6; i++){
            nextChessCircle[i] = nullptr;
        }
    }
    ChessCircle(bool flag,int x,int y){
        setChessCircle(flag,x,y);
        for(int i=0 ; i < 6; i++){
            nextChessCircle[i] = nullptr;
        }
    }
    void setChessCircle(bool flag,int x,int y){
        if(chesschecker_inner.IsLegalPosition(x,y) == false)
            return;
        dumpOrchessFlag = flag;
        this->x = x;
        this->y = y;
    }
    void setChessNextCircle(      ChessCircle *nextChessCircle0,
                                  ChessCircle *nextChessCircle1,
                                  ChessCircle *nextChessCircle2,
                                  ChessCircle *nextChessCircle3,
                                  ChessCircle *nextChessCircle4,
                                  ChessCircle *nextChessCircle5) ;

    bool getFlag(){
        return  dumpOrchessFlag;
    }
    ChessColor getColor(){
        return color;
    }
    int getx(){
        return x;
    }
    int gety(){
        return y;
    }
    void setFlag(bool flag){
        dumpOrchessFlag = flag;
    }
    void setcolor(ChessColor color){
        this->color = color;
    }
    void setpos(int x,int y){
        if(chesschecker_inner.IsLegalPosition(x,y) == false)
            return;
        this->x = x;
        this->y = y;
    }

};
struct CircleReturn{
    Point curPoint;
    ChessColor  curColor;
    int CircleMap_i=0;//该指保存在CircleMap中的位置，方便查找该节点相关信息
    int Map_x;
    int Map_y;
    bool Down_Chess_flag;//true该位置可以放下棋子，false该地方不能方棋子
    NextType nextType;
    CircleReturn(Point curPoint,ChessColor curColor){
        this->curPoint = curPoint;
        this->curColor = curColor;
        Map_x =0;
        Map_y =0;
        Down_Chess_flag = false;
        nextType = SELF;
    }
    CircleReturn(){

    }
    ~CircleReturn(){

    }
};

#endif //JUMP0_CHESSINCLUDE_H
