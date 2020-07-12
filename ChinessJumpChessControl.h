//
// Created by litm on 2020/3/15.
//

#ifndef JUMP0_CHINESSJUMPCHESSCONTROL_H
#define JUMP0_CHINESSJUMPCHESSCONTROL_H

//#include "ChessInclude.h"
#include "CheckersMapLimitCheck.h"
#include "ChessNode.h"
#include <list>
#include <map>
using namespace cv;
using namespace std;


class ChinessJumpChessControl {
private:
    Mat chessmapmat;
    CheckersMapLimitCheck* checker;
    map<int,list<CircleReturn>> *MapChessControlMemory;
    ChessNode *head;



public:
    void init(ChessNode *p);
    ChinessJumpChessControl(Mat chessmapmat, CheckersMapLimitCheck *pcheck,map<int,list<CircleReturn>> *pMemory){
        this->chessmapmat = chessmapmat;
        this->checker = pcheck;
        this->MapChessControlMemory = pMemory;
    }
    bool CanJumpFun(Point source,Point dest);//判断当前的棋子是否落地点是否合法，只有合法才能移动
    bool ProbableJumpPathALLShow(Point source,int Map_i,int Type=0,list<Point> *p_get= nullptr);//显示当前棋子可能的跳动的路径。Type=0缺省是0的话，则显示路径，大于0则擦除之前显示的路径。
    void FindPathList(int Map_id,map<int,list<CircleReturn>> &circlemap) ;
    map<string,list<CircleReturn>> * FindPathList(int Map_id);
    CircleReturn * GetNodeInfo(int Map_id);
    list<int> * GetMatchList(ChessColor *sec,int sec_count);

};


#endif //JUMP0_CHINESSJUMPCHESSCONTROL_H
