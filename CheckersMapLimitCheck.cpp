//
// Created by litm on 2020/2/2.
//

#include <cstdlib>
#include "CheckersMapLimitCheck.h"
bool CheckersMapLimitCheck::IsLegalPosition(int x ,int y) {
    if(x<1 || x>MAX_X|| (y <1 || y >MAX_Y)){
        return false;
    }


    if((y< mapXCountToYMin2Max[x-1][0])||
       (y> mapXCountToYMin2Max[x-1][1])){
        return false;
    }
    return true;
}
int CheckersMapLimitCheck::getDistanceFromRED_5_1_point_Y(int x, int y) {
    if(x <1 || x > MAX_X) return  0;
    if(y <1 || y > MAX_Y) return  0;
    return  y - 1;
}
//返回相对于RED(5,1)的其他坐标 x方向的🍕背书，是在CheckersUI.h中定义的那个xwidth的多少陪
int CheckersMapLimitCheck::getDistanceFromRED_5_1_point_X(int x, int y) {

    if(x <1 || x > MAX_X) return  0;
    if(y <1 || y > MAX_Y) return  0;
    if(IsLegalPosition(x,y) == false)return 0;
    if(x == 13 && y == 5) return -12;
    if(x == 13 && y == 6) return -11;
    if(x == 12 && y == 5) return -10;
    if(x == 13 && y == 7) return -10;
    if(x == 12 && y == 6) return -9;
    if(x == 13 && y == 8) return -9;
    if(x == 11 && y == 5) return -8;
    if(x == 12 && y == 7) return -8;
    if(x == 13 && y == 9) return -8;
    if(x == 11 && y == 6) return -7;
    if(x == 12 && y == 8) return -7;
    if(x == 10 && y == 5) return -6;
    if(x == 11 && y == 7) return -6;

    if(x == 1 && y == 5) return 12;
    if(x == 2 && y == 6) return 11;
    if(x == 2 && y == 5) return 10;
    if(x == 3 && y == 7) return 10;
    if(x == 3 && y == 6) return 9;
    if(x == 4 && y == 8) return 9;
    if(x == 3 && y == 5) return 8;
    if(x == 4 && y == 7) return 8;
    if(x == 5 && y == 9) return 8;
    if(x == 4 && y == 6) return 7;
    if(x == 5 && y == 8) return 7;
    if(x == 4 && y == 5) return 6;
    if(x == 5 && y == 7) return 6;

    if(x == 5) return  y - 1;
    if(x == 6) return  (y-2) - 1 ;
    if(x == 7) return  (y-3) - 2 ;
    if(x == 8) return  (y-4) - 3 ;
    if(x == 9) return  (y-5) - 4 ;
    if(x == 10) return (y-6) - 5 ;
    if(x == 11) return (y-7) - 6 ;
    if(x == 12) return (y-8) - 7 ;
    if(x == 13) return (y-9) - 8 ;
    if(x == 14) return (y-10)- 9 ;
    if(x == 15) return (y-11)- 10;
    if(x == 16) return (y-12)- 11;
    if(x == 17) return (y-13)- 12;

    return 0;

}
ChessColor CheckersMapLimitCheck::getCircleColor(int x, int y) {
    return getChessColor(x,y);
}
ChessColor CheckersMapLimitCheck::getChessColor(int x, int y) {
    if(x==5 && y >=1 && y <=4 )return RED;
    if(x==6 && y>=2 && y <= 4) return RED;
    if(x==7 && y>=3 && y <=4) return RED;
    if(x==8 && y==4) return RED;

    if(y ==5 && x>=10 && x <= 13) return GREEN;
    if(y ==6 && x>=11 && x <= 13) return GREEN;
    if(y ==7 && x>=12 && x <=13) return GREEN;
    if(y ==8 && x==13)return GREEN;

    if(x ==14 && y>=10 && y<=13) return ROSERED;
    if(x==15 && y>=11 && y<=13) return ROSERED;
    if(x==16 && y>=12 && y<=13) return ROSERED;
    if(x==17 && y==13 )return ROSERED;

    if(y ==14 && x>=10 && x<=13) return ORANGE;
    if(y ==15 && x>=11 && x<=13) return ORANGE;
    if(y ==16 && x>=12 && x<=13) return ORANGE;
    if(y ==17 && x==13) return ORANGE;

    if(x==5 && y>=10 && y<=13) return WHITE;
    if(x==6 && y>=11 && y<=13) return WHITE;
    if(x==7 && y>=12 && y<=13) return WHITE;
    if(x==8 && y==13) return WHITE;

    if(y==5 && x>=1 && x<=4) return BLUE;
    if(y==6 && x>=2 && x<=4) return BLUE;
    if(y==7 && x>=3 && x<=4) return BLUE;
    if(y==8 && x==4) return BLUE;

    return SHARELIGHTGREEN;

}
list<Point>* CheckersMapLimitCheck::getChessNeighbourList(Point curnode) {

    list<Point> *p = new list<Point>();
    Point p_top_left,p_top_right;
    Point p_middle_left,p_middle_right;
    Point p_down_left,p_down_right;

    p_top_left.x = curnode.x;
    p_top_left.y = curnode.y -1;
    if(IsLegalPosition(p_top_left.x,p_top_left.y)){
        p->push_back(Point(p_top_left));
    }

    p_top_right.x = curnode.x -1;
    p_top_right.y = curnode.y -1;
    if(IsLegalPosition(p_top_right.x,p_top_right.y)){
        p->push_back(Point(p_top_right));
    }

    p_middle_left.x = curnode.x +1;
    p_middle_left.y = curnode.y;
    if(IsLegalPosition(p_middle_left.x,p_middle_left.y)){
        p->push_back(Point(p_middle_left));
    }

    p_middle_right.x = curnode.x -1;
    p_middle_right.y = curnode.y;
    if(IsLegalPosition(p_middle_right.x,p_middle_right.y)){
        p->push_back(Point(p_middle_right));
    }

    p_down_left.x = curnode.x +1;
    p_down_left.y = curnode.y +1;
    if(IsLegalPosition(p_down_left.x,p_down_left.y)){
        p->push_back(Point(p_down_left));
    }

    p_down_right.x = curnode.x;
    p_down_right.y = curnode.y +1 ;
    if(IsLegalPosition(p_down_right.x,p_down_right.y)){
        p->push_back(Point(p_down_right));
    }


    return  p;
}
int CheckersMapLimitCheck::getCircleMapPostion(int x, int y) {
    if(x<1 || y <1 || x > 17)
        return -1;
    int iResult=-2;
    for(int i=0; i < MAX_CHESS;i++){
        if(CircleMap[i][0] == x &&
           CircleMap[i][1] == y)
            iResult = i;
            break;
    }
    return iResult;
}