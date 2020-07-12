//
// Created by litm on 2020/1/5.
//

#ifndef JUMP0_JUMP_H
#define JUMP0_JUMP_H

#include "jump.h"
#include "CheckersUI.h"
#include "CheckersMapLimitCheck.h"
#include "ChessInclude.h"
#include "CheckersImageObject.h"
#include "MouseMoveChessOpt.h"

//定义棋盘大小
//毫米

class CheckersHoneycomb{
private:
    ChessCircle honecombCircle[SHARELIGHTGREEN_COUNT];
    ChessCircle triangleCircle[6][10];
public:
    CheckersHoneycomb(){
        //这里的数据初始化和数组是重复进行了，这个是为了建立一个对象的关系，那个数组，是为了校验或者更快的搜索
        initTriangleCircle(); // 1.必须是这个顺序否则前后初始化失败
        init();//2.这个需要在前面一个后面调用，因为里面使用了前一个的初始化数据

    }
    void init();
    void initTriangleCircle();
    ChessCircle* gethonecomb(int n){
        if(n<0 || n > SHARELIGHTGREEN_COUNT)
            return nullptr;
        return &honecombCircle[n];
    }
    ChessCircle* getTriangleCircle(){
        return &triangleCircle[0][0];
    }
    ChessCircle* getTriangleCircle(ChessColor color){
         int n = color - 1;
            if(n <0 || n >6)
                return nullptr;
        return &triangleCircle[n][0];

    }

    //全部棋盘搜索正确的位置的对象
    ChessCircle* getCC(int x, int y){

        ChessCircle * p= nullptr;
        p = getChessCircle(x,y);
        if(p == nullptr){
            p = gethonecomb(x,y);
        }
        return  p;
    }
    ChessCircle* getChessCircle(int x, int y){
        if(x<0 || x > MAX_X)
            return nullptr;
        if(y<0 || y > MAX_Y)
            return nullptr;
        int nx ,ny;
        for(int i=0; i< 6; i++){
            for(int j=0 ; j < 10; j++){
                nx = triangleCircle[i][j].getx();
                ny = triangleCircle[i][j].gety();
                if(nx == x || ny == y ) {
                    return &triangleCircle[i][j];
                }
            }
        }
            return nullptr;
    }
    ChessCircle* gethonecomb(int x,int y){
        if(x<0 || x > MAX_X)
            return nullptr;
        if(y<0 || y > MAX_Y)
            return nullptr;
        int nx,ny;
        for(int i=0; i < SHARELIGHTGREEN_COUNT; i++ ){
            nx = honecombCircle[i].getx();
            ny = honecombCircle[i].gety();

            if(nx == x && ny == y)
                return  &triangleCircle[i][0];
        }
        return nullptr;
    }

};
//每个顶角三角形的定义
class CheckersTriangle {
private:
    Chess triangleChess[10];//顶部三角形跳棋情况
    bool usedflag;//当前的三角形是否被对弈双方使用的标志
public:
    CheckersTriangle(){
        usedflag = false;
    }
    CheckersTriangle(ChessColor chesscolor){
        init(chesscolor);
    }
    void init(ChessColor chesscolor);
    Chess getChess(int inum){
        if(inum<1 || inum >10) {
            inum = 1;
        }

        return triangleChess[inum-1];
    }

};

enum ContestType{ONE,TWO,THREE};
enum ContestWhichColor{REDORANGE,WHITEGREEN,BLUEROSERED,NOCONTEST};
//保存对弈双方的上下文信息的对象
class ChessContestContext{
private:
    int triangleContest[3][2];//对弈数组，标记对弈双方颜色，没有对弈的则标记-1
    ContestType ctype;//记录是1对1，2对2，还是3对3
    int iType=0;
    ContestWhichColor whichContest[3];//记录是哪种颜色双方在对弈

public:
    //default 3对3对弈
    ChessContestContext();
    //单独对弈，1对1 对弈
    ChessContestContext(enum ChessColor color);
    //单独对弈，1对1 对弈
    ChessContestContext(ChessColor color,ChessColor twocolor);

    //初始化整个棋盘信息
    void init(){

    }

    ContestWhichColor getWhichColor(int i);
    ContestType  getType();

    void getCheckersTriangle(CheckersTriangle trinangle[],int n = 6);
};




class CheckersMap{
private:
    ChessContestContext* context;
    CheckersTriangle trinangle[6];//初始化的三角性棋子和空位的情况
    CheckersHoneycomb chessciclemap;
    int checkersmap[MAX_X][MAX_Y];// 为了加快搜索速度而设计的一个代表整个棋盘的是否有棋子的数组


private:
    void initMap();
public:
    CheckersMap(){
        this->context = new ChessContestContext();
        init();
    }
    ~CheckersMap(){
       // delete context;

    }
    CheckersMap(ChessContestContext* pcontext){
        this->context = pcontext;
        init();
    }

    bool init(){
        if(this->context == nullptr)
            return false;
        context->init();
        context->getCheckersTriangle(trinangle);
        initMap();
        printmap();
        return true;
    }
    void printmap();
    void search(){

    }
};



#endif //JUMP0_JUMP_H
