//
// Created by litm on 2020/1/26.
//
#include <cstdio>
#include "jump.h"


int Chess::getx(){
    return  x;
}
int Chess::gety() {
    return  y;
}
ChessColor  Chess::getcolor(){
    return color;
}
void Chess::setpos(int x, int y) {
        if(chesschecker.IsLegalPosition(x,y) == false)
            return;
        this->x = x;
        this->y = y;
}
void Chess::setcolor(ChessColor color) {
    this->color = color;
}
ChessContestContext::ChessContestContext() {
    triangleContest[0][0] = RED;
    triangleContest[0][1] = ORANGE;

    triangleContest[1][0] = BLUE;
    triangleContest[1][1] = ROSERED;

    triangleContest[2][0] = WHITE;
    triangleContest[2][1] = GREEN;

    whichContest[0] = REDORANGE;
    whichContest[1] = WHITEGREEN;
    whichContest[2] = BLUEROSERED;
    ctype = THREE;
}
ChessContestContext::ChessContestContext(enum ChessColor color) {
    //初始化
    triangleContest[0][0] = -1;
    triangleContest[0][1] = -1;

    triangleContest[1][0] = -1;
    triangleContest[1][1] = -1;

    triangleContest[2][0] = -1;
    triangleContest[2][1] = -1;

    whichContest[0] = NOCONTEST;
    whichContest[1] = NOCONTEST;
    whichContest[2] = NOCONTEST;
    ctype = ONE;
    switch (color){
        case RED:
            triangleContest[0][0] = RED;
            triangleContest[0][1] = ORANGE;
            whichContest[0] = REDORANGE;
            break;
        case ORANGE:
            triangleContest[0][0] = RED;
            triangleContest[0][1] = ORANGE;
            whichContest[0] = REDORANGE;
            break;
        case BLUE:
            triangleContest[1][0] = BLUE;
            triangleContest[1][1] = ROSERED;
            whichContest[2] = BLUEROSERED;
            break;
        case ROSERED:
            triangleContest[1][0] = BLUE;
            triangleContest[1][1] = ROSERED;
            whichContest[2] = BLUEROSERED;
            break;
        case WHITE:
            triangleContest[2][0] = WHITE;
            triangleContest[2][1] = GREEN;
            whichContest[1] = WHITEGREEN;
            break;
        case GREEN:
            triangleContest[2][0] = WHITE;
            triangleContest[2][1] = GREEN;
            whichContest[1] = WHITEGREEN;
            break;
        default:
            triangleContest[0][0] = RED;
            triangleContest[0][1] = ORANGE;

            triangleContest[1][0] = BLUE;
            triangleContest[1][1] = ROSERED;

            triangleContest[2][0] = WHITE;
            triangleContest[2][1] = GREEN;

            whichContest[0] = REDORANGE;
            whichContest[1] = WHITEGREEN;
            whichContest[2] = BLUEROSERED;

            ctype = THREE;
    }
}
ChessContestContext::ChessContestContext(ChessColor color, ChessColor twocolor) {
    //初始化
    triangleContest[0][0] = -1;
    triangleContest[0][1] = -1;

    triangleContest[1][0] = -1;
    triangleContest[1][1] = -1;

    triangleContest[2][0] = -1;
    triangleContest[2][1] = -1;

    whichContest[0] = NOCONTEST;
    whichContest[1] = NOCONTEST;
    whichContest[2] = NOCONTEST;

    ctype = TWO;
    for (int i = 0; i < 2; i++) {
        switch (color) {
            case RED:
                triangleContest[0][0] = RED;
                triangleContest[0][1] = ORANGE;
                whichContest[0] = REDORANGE;
                break;
            case ORANGE:
                triangleContest[0][0] = RED;
                triangleContest[0][1] = ORANGE;
                whichContest[0] = REDORANGE;
                break;
            case BLUE:
                triangleContest[1][0] = BLUE;
                triangleContest[1][1] = ROSERED;
                whichContest[2] = BLUEROSERED;
                break;
            case ROSERED:
                triangleContest[1][0] = BLUE;
                triangleContest[1][1] = ROSERED;
                whichContest[2] = BLUEROSERED;
                break;
            case WHITE:
                triangleContest[2][0] = WHITE;
                triangleContest[2][1] = GREEN;
                whichContest[1] = WHITEGREEN;
                break;
            case GREEN:
                triangleContest[2][0] = WHITE;
                triangleContest[2][1] = GREEN;
                whichContest[1] = WHITEGREEN;
                break;
            default:
                triangleContest[0][0] = RED;
                triangleContest[0][1] = ORANGE;

                triangleContest[1][0] = BLUE;
                triangleContest[1][1] = ROSERED;

                triangleContest[2][0] = WHITE;
                triangleContest[2][1] = GREEN;

                whichContest[0] = REDORANGE;
                whichContest[1] = WHITEGREEN;
                whichContest[2] = BLUEROSERED;

                ctype = THREE;
        }
    }
    int isOneOrTwoOrThree = 0;
    for (int j = 0; j < 3; j++) {
        if (whichContest[j] != NOCONTEST) {
            isOneOrTwoOrThree++;
        }
    }
    if (isOneOrTwoOrThree == 2) {
        ctype = TWO;
        iType = 2;
    }
    else if(isOneOrTwoOrThree == 1 ) {
        ctype = ONE;
        iType = 1;
    }
    else {
        ctype = THREE;
        iType = 3;
    }
}
ContestWhichColor ChessContestContext::getWhichColor(int i) {
    if(i<0 ||i >2)
        i =0;
    return whichContest[i];
}
ContestType ChessContestContext::getType() {
    return ctype;
}

void ChessContestContext::getCheckersTriangle(CheckersTriangle trinangle[],int n) {



        switch (ctype){
            case THREE:{
                for (int i = 0,j=0; i < 3 && n >=6 ; i++) {
                    if (triangleContest[i][0] != -1 && triangleContest[i][1] != -1) {
                        trinangle[j].init((ChessColor) triangleContest[i][0]);
                        j++;
                        trinangle[j].init((ChessColor) triangleContest[i][1]);
                        j++;
                    }
                }
                break;
            }
            case TWO:{
                for (int i = 0,j=0; i < 2 && n >=4 ; i++) {
                    if (triangleContest[i][0] != -1 && triangleContest[i][1] != -1) {
                        trinangle[j].init((ChessColor) triangleContest[i][0]);
                        j++;
                        trinangle[j].init((ChessColor) triangleContest[i][1]);
                        j++;
                    }
                }
                break;
            }
            case ONE:{
                for (int i = 0,j=0; i < 1 && n >=2 ; i++) {
                    if (triangleContest[i][0] != -1 && triangleContest[i][1] != -1) {
                        trinangle[j].init((ChessColor) triangleContest[i][0]);
                        j++;
                        trinangle[j].init((ChessColor) triangleContest[i][1]);
                        j++;
                    }

                }
                break;
            }
        }


}
void CheckersHoneycomb::init() {
    for(int i=0; i < SHARELIGHTGREEN_COUNT; i++){
        honecombCircle[i].setcolor(SHARELIGHTGREEN);
        honecombCircle[i].setFlag(false);
    }

    //所有第一排的和三角形接触的空缺，在三角性的里面初始化上下游的关系

    //RED作为正北，三角形第一个不放初始化棋子的第一行的坐标
    honecombCircle[0].setpos(9,5);
    honecombCircle[0].setChessNextCircle(getCC(8,4),getCC(10,5),getCC(10,6),getCC(8,5), getCC(9,6),nullptr);
    honecombCircle[1].setpos(8,5);
    honecombCircle[1].setChessNextCircle(getCC(8,4),getCC(7,4),getCC(9,5),getCC(7,5), getCC(9,6),getCC(8,6));
    honecombCircle[2].setpos(7,5);
    honecombCircle[2].setChessNextCircle(getCC(6,4),getCC(7,4),getCC(8,5),getCC(6,5), getCC(7,6),getCC(8,6));
    honecombCircle[3].setpos(6,5);
    honecombCircle[3].setChessNextCircle(getCC(5,4),getCC(6,4),getCC(7,5),getCC(6,5), getCC(7,6),getCC(6,6));
    honecombCircle[4].setpos(5,5);
    honecombCircle[4].setChessNextCircle(getCC(5,4),getCC(4,5),getCC(6,5),getCC(6,6), getCC(5,6), nullptr);
    //第二行
    honecombCircle[5].setpos(10,6);
    honecombCircle[5].setChessNextCircle(getCC(11,6),getCC(10,5),getCC(9,5),getCC(11,7), getCC(10,7),getCC(9,6));
    honecombCircle[6].setpos(9,6);
    honecombCircle[6].setChessNextCircle(getCC(9,5),getCC(8,5),getCC(10,6),getCC(8,6), getCC(10,7), getCC(9,7));
    honecombCircle[7].setpos(8,6);
    honecombCircle[7].setChessNextCircle(getCC(8,5),getCC(7,5),getCC(9,6),getCC(7,6), getCC(8,7), getCC(9,7));
    honecombCircle[8].setpos(7,6);
    honecombCircle[8].setChessNextCircle(getCC(7,5),getCC(6,5),getCC(8,6),getCC(6,6), getCC(8,7), getCC(7,7));
    honecombCircle[9].setpos(6,6);
    honecombCircle[9].setChessNextCircle(getCC(6,5),getCC(5,5),getCC(7,6),getCC(5,6), getCC(7,7), getCC(6,7));
    honecombCircle[10].setpos(5,6);
    honecombCircle[10].setChessNextCircle(getCC(5,5),getCC(4,5),getCC(6,6),getCC(4,6), getCC(6,7), getCC(5,7));
    //第三行
    honecombCircle[11].setpos(11,7);
    honecombCircle[11].setChessNextCircle(getCC(11,6),getCC(10,6),getCC(12,7),getCC(10,7), getCC(12,8), getCC(11,8));
    honecombCircle[12].setpos(10,7);
    honecombCircle[12].setChessNextCircle(getCC(10,6),getCC(9,6),getCC(11,7),getCC(9,7), getCC(11,8), getCC(10,8));
    honecombCircle[13].setpos(9,7);
    honecombCircle[13].setChessNextCircle(getCC(9,6),getCC(8,6),getCC(10,7),getCC(8,7), getCC(10,8), getCC(9,8));
    honecombCircle[14].setpos(8,7);
    honecombCircle[14].setChessNextCircle(getCC(8,6),getCC(7,6),getCC(9,7),getCC(7,7), getCC(9,8), getCC(8,8));
    honecombCircle[15].setpos(7,7);
    honecombCircle[15].setChessNextCircle(getCC(7,6),getCC(6,6),getCC(8,7),getCC(6,7), getCC(8,8), getCC(7,8));
    honecombCircle[16].setpos(6,7);
    honecombCircle[16].setChessNextCircle(getCC(6,6),getCC(5,6),getCC(7,7),getCC(5,7), getCC(7,8), getCC(6,8));
    honecombCircle[17].setpos(5,7);
    honecombCircle[17].setChessNextCircle(getCC(5,6),getCC(4,6),getCC(6,7),getCC(4,7), getCC(6,8), getCC(5,8));
    //第四行
    honecombCircle[18].setpos(12,8);
    honecombCircle[18].setChessNextCircle(getCC(12,7),getCC(11,7),getCC(13,8),getCC(11,8), getCC(13,9), getCC(12,9));
    honecombCircle[19].setpos(11,8);
    honecombCircle[19].setChessNextCircle(getCC(11,7),getCC(10,7),getCC(12,8),getCC(10,8), getCC(12,9), getCC(11,9));
    honecombCircle[20].setpos(10,8);
    honecombCircle[20].setChessNextCircle(getCC(10,7),getCC(9,7),getCC(11,8),getCC(9,8), getCC(11,9), getCC(10,9));
    honecombCircle[21].setpos(9,8);
    honecombCircle[21].setChessNextCircle(getCC(9,7),getCC(8,7),getCC(10,8),getCC(8,8), getCC(10,9), getCC(9,9));
    honecombCircle[22].setpos(8,8);
    honecombCircle[22].setChessNextCircle(getCC(8,7),getCC(7,7),getCC(9,8),getCC(7,8), getCC(9,9), getCC(8,9));
    honecombCircle[23].setpos(7,8);
    honecombCircle[23].setChessNextCircle(getCC(7,7),getCC(6,7),getCC(8,8),getCC(6,8), getCC(8,9), getCC(7,9));
    honecombCircle[24].setpos(6,8);
    honecombCircle[24].setChessNextCircle(getCC(6,7),getCC(5,7),getCC(7,8),getCC(5,8), getCC(7,9), getCC(6,9));
    honecombCircle[25].setpos(5,8);
    honecombCircle[25].setChessNextCircle(getCC(5,7),getCC(4,7),getCC(6,8),getCC(4,8), getCC(6,9), getCC(5,9));
    //第五行
    honecombCircle[26].setpos(13,9);
    honecombCircle[26].setChessNextCircle(getCC(13,8),getCC(12,8),getCC(12,9),getCC(14,10), getCC(13,10), nullptr);
    honecombCircle[27].setpos(12,9);
    honecombCircle[27].setChessNextCircle(getCC(12,8),getCC(11,8),getCC(13,9),getCC(11,9), getCC(13,10), getCC(12,10));
    honecombCircle[28].setpos(11,9);
    honecombCircle[28].setChessNextCircle(getCC(11,8),getCC(10,8),getCC(12,9),getCC(10,9), getCC(12,10), getCC(11,10));
    honecombCircle[29].setpos(10,9);
    honecombCircle[29].setChessNextCircle(getCC(10,8),getCC(9,8),getCC(11,9),getCC(9,9), getCC(11,10), getCC(10,10));
    honecombCircle[30].setpos(9,9);
    honecombCircle[30].setChessNextCircle(getCC(9,8),getCC(8,8),getCC(10,9),getCC(8,9), getCC(10,10), getCC(9,10));
    honecombCircle[31].setpos(8,9);
    honecombCircle[31].setChessNextCircle(getCC(8,8),getCC(7,8),getCC(9,9),getCC(7,9), getCC(9,10), getCC(8,10));
    honecombCircle[32].setpos(7,9);
    honecombCircle[32].setChessNextCircle(getCC(7,8),getCC(6,8),getCC(8,9),getCC(6,9), getCC(8,10), getCC(7,10));
    honecombCircle[33].setpos(6,9);
    honecombCircle[33].setChessNextCircle(getCC(6,8),getCC(5,8),getCC(7,9),getCC(5,9), getCC(7,10), getCC(6,10));
    honecombCircle[34].setpos(5,9);
    honecombCircle[34].setChessNextCircle(getCC(5,8),getCC(4,8),getCC(6,9),getCC(6,10), getCC(5,10), nullptr);
    //第六行
    honecombCircle[35].setpos(13,10);
    honecombCircle[35].setChessNextCircle(getCC(13,9),getCC(12,9),getCC(14,10),getCC(12,10), getCC(14,11), getCC(13,11));
    honecombCircle[36].setpos(12,10);
    honecombCircle[36].setChessNextCircle(getCC(12,9),getCC(11,9),getCC(13,10),getCC(11,10), getCC(13,11), getCC(12,11));
    honecombCircle[37].setpos(11,10);
    honecombCircle[37].setChessNextCircle(getCC(11,9),getCC(10,9),getCC(12,10),getCC(10,10), getCC(12,11), getCC(11,11));
    honecombCircle[38].setpos(10,10);
    honecombCircle[38].setChessNextCircle(getCC(10,9),getCC(9,9),getCC(11,10),getCC(9,10), getCC(11,11), getCC(10,11));
    honecombCircle[39].setpos(9,10);
    honecombCircle[39].setChessNextCircle(getCC(9,9),getCC(8,9),getCC(10,10),getCC(8,10), getCC(10,11), getCC(9,11));
    honecombCircle[40].setpos(8,10);
    honecombCircle[40].setChessNextCircle(getCC(8,9),getCC(7,9),getCC(9,10),getCC(7,10), getCC(9,11), getCC(8,11));
    honecombCircle[41].setpos(7,10);
    honecombCircle[41].setChessNextCircle(getCC(7,9),getCC(6,9),getCC(8,10),getCC(6,10), getCC(8,11), getCC(7,11));
    honecombCircle[42].setpos(6,10);
    honecombCircle[42].setChessNextCircle(getCC(6,9),getCC(5,9),getCC(7,10),getCC(5,10), getCC(7,11), getCC(6,11));
    //第七行
    honecombCircle[43].setpos(13,11);
    honecombCircle[43].setChessNextCircle(getCC(13,10),getCC(12,10),getCC(14,11),getCC(12,11), getCC(14,12), getCC(13,12));
    honecombCircle[44].setpos(12,11);
    honecombCircle[44].setChessNextCircle(getCC(12,10),getCC(11,10),getCC(13,11),getCC(11,11), getCC(13,12), getCC(12,12));
    honecombCircle[45].setpos(11,11);
    honecombCircle[45].setChessNextCircle(getCC(11,10),getCC(10,10),getCC(12,11),getCC(10,11), getCC(12,12), getCC(11,12));
    honecombCircle[46].setpos(10,11);
    honecombCircle[46].setChessNextCircle(getCC(10,10),getCC(9,10),getCC(11,11),getCC(9,11), getCC(11,12), getCC(10,12));
    honecombCircle[47].setpos(9,11);
    honecombCircle[47].setChessNextCircle(getCC(9,10),getCC(8,10),getCC(10,11),getCC(8,11), getCC(10,12), getCC(9,12));
    honecombCircle[48].setpos(8,11);
    honecombCircle[48].setChessNextCircle(getCC(8,10),getCC(7,10),getCC(9,11),getCC(7,11), getCC(9,12), getCC(8,12));
    honecombCircle[49].setpos(7,11);
    honecombCircle[49].setChessNextCircle(getCC(7,10),getCC(6,10),getCC(8,11),getCC(6,11), getCC(8,12), getCC(7,12));
    //第八行
    honecombCircle[50].setpos(13,12);
    honecombCircle[50].setChessNextCircle(getCC(13,11),getCC(12,11),getCC(14,12),getCC(12,12), getCC(14,13), getCC(13,13));
    honecombCircle[51].setpos(12,12);
    honecombCircle[51].setChessNextCircle(getCC(12,11),getCC(11,11),getCC(14,12),getCC(11,12), getCC(13,13), getCC(12,13));
    honecombCircle[52].setpos(11,12);
    honecombCircle[52].setChessNextCircle(getCC(11,11),getCC(10,11),getCC(14,12),getCC(10,12), getCC(12,13), getCC(11,13));
    honecombCircle[53].setpos(10,12);
    honecombCircle[53].setChessNextCircle(getCC(10,11),getCC(9,11),getCC(14,12),getCC(9,12), getCC(11,13), getCC(10,13));
    honecombCircle[54].setpos(9,12);
    honecombCircle[54].setChessNextCircle(getCC(9,11),getCC(8,11),getCC(14,12),getCC(8,12), getCC(10,13), getCC(9,13));
    honecombCircle[55].setpos(8,12);
    honecombCircle[55].setChessNextCircle(getCC(8,11),getCC(7,11),getCC(14,12),getCC(7,12), getCC(9,13), getCC(8,13));
    //第九行 也就是Orange颜色对应的不放棋子最接近的一行
    honecombCircle[56].setpos(13,13);
    honecombCircle[56].setChessNextCircle(getCC(13,12),getCC(12,12),getCC(14,13),getCC(12,13), getCC(13,14), nullptr);
    honecombCircle[57].setpos(12,13);
    honecombCircle[57].setChessNextCircle(getCC(12,12),getCC(11,12),getCC(13,13),getCC(11,13), getCC(12,14), getCC(13,14));
    honecombCircle[58].setpos(11,13);
    honecombCircle[58].setChessNextCircle(getCC(11,12),getCC(10,12),getCC(12,13),getCC(10,13), getCC(11,14), getCC(12,14));
    honecombCircle[59].setpos(10,13);
    honecombCircle[59].setChessNextCircle(getCC(10,12),getCC(9,12),getCC(11,13),getCC(9,13), getCC(10,14), getCC(11,14));
    honecombCircle[60].setpos(9,13);
    honecombCircle[60].setChessNextCircle(getCC(9,12),getCC(8,12),getCC(10,13),getCC(8,13), getCC(10,14), nullptr);
}

void CheckersHoneycomb::initTriangleCircle() {

        //case RED://10个棋子和空位的初始化
            for(int i=0; i< 10; i++){
                triangleCircle[0][i].setcolor(RED);
            }
            triangleCircle[0][0].setpos(5,1);
            triangleCircle[0][0].setFlag(true);


            triangleCircle[0][1].setpos(5,2);
            triangleCircle[0][1].setFlag(true);

            triangleCircle[0][2].setpos(5,3);
            triangleCircle[0][2].setFlag(true);

            triangleCircle[0][3].setpos(5,4);
            triangleCircle[0][3].setFlag(true);

            triangleCircle[0][4].setpos(6,2);
            triangleCircle[0][4].setFlag(true);

            triangleCircle[0][5].setpos(6,3);
            triangleCircle[0][5].setFlag(true);

            triangleCircle[0][6].setpos(6,4);
            triangleCircle[0][6].setFlag(true);

            triangleCircle[0][7].setpos(7,3);
            triangleCircle[0][7].setFlag(true);

            triangleCircle[0][8].setpos(7,4);
            triangleCircle[0][8].setFlag(true);

            triangleCircle[0][9].setpos(8,4);
            triangleCircle[0][9].setFlag(true);

            //空缺连接位置设置
            triangleCircle[0][0].setChessNextCircle(getChessCircle(5,2),getChessCircle(6,2),nullptr, nullptr, nullptr, nullptr);
            triangleCircle[0][1].setChessNextCircle(getChessCircle(5,1),getChessCircle(6,2),getChessCircle(6,3), getChessCircle(5,3), nullptr, nullptr);
            triangleCircle[0][2].setChessNextCircle(getChessCircle(5,2),getChessCircle(6,3),getChessCircle(6,4), getChessCircle(5,4), nullptr, nullptr);
            triangleCircle[0][3].setChessNextCircle(getChessCircle(5,3),getChessCircle(6,4),gethonecomb(6,5),gethonecomb(5,5), nullptr, nullptr);

            triangleCircle[0][4].setChessNextCircle(getChessCircle(5,1),getChessCircle(5,2),getChessCircle(6,3), getChessCircle(7,3), nullptr, nullptr);
            triangleCircle[0][5].setChessNextCircle(getChessCircle(5,2),getChessCircle(6,2),getChessCircle(5,3), getChessCircle(7,3), getChessCircle(7,4), getChessCircle(6,4));
            triangleCircle[0][6].setChessNextCircle(getChessCircle(5,3),getChessCircle(6,3),getChessCircle(7,4),getChessCircle(5,4), gethonecomb(7,5), gethonecomb(6,5));

            triangleCircle[0][7].setChessNextCircle(getChessCircle(6,2),getChessCircle(6,3),getChessCircle(7,4), getChessCircle(8,4),nullptr,nullptr);
            triangleCircle[0][8].setChessNextCircle(getChessCircle(7,3),getChessCircle(6,3),getChessCircle(8,4),getChessCircle(6,4), gethonecomb(8,5), gethonecomb(7,5));

            triangleCircle[0][9].setChessNextCircle(getChessCircle(7,3),getChessCircle(7,4),gethonecomb(8,5),gethonecomb(9,5),nullptr, nullptr);

        //  ORANGE:
            for(int i=0; i< 10; i++){
                triangleCircle[4][i].setcolor(ORANGE);
            }
            triangleCircle[4][0].setpos(10,14);
            triangleCircle[4][0].setFlag(true);

            triangleCircle[4][1].setpos(11,14);
            triangleCircle[4][1].setFlag(true);

            triangleCircle[4][2].setpos(12,14);
            triangleCircle[4][2].setFlag(true);

            triangleCircle[4][3].setpos(13,14);
            triangleCircle[4][3].setFlag(true);

            triangleCircle[4][4].setpos(11,15);
            triangleCircle[4][4].setFlag(true);

            triangleCircle[4][5].setpos(12,15);
            triangleCircle[4][5].setFlag(true);

            triangleCircle[4][6].setpos(13,15);
            triangleCircle[4][6].setFlag(true);

            triangleCircle[4][7].setpos(12,16);
            triangleCircle[4][7].setFlag(true);

            triangleCircle[4][8].setpos(13,16);
            triangleCircle[4][8].setFlag(true);

            triangleCircle[4][9].setpos(13,17);
            triangleCircle[4][9].setFlag(true);

            //空缺连接位置设置
            triangleCircle[4][9].setChessNextCircle(getChessCircle(13,16),getChessCircle(12,16),nullptr, nullptr, nullptr, nullptr);
            triangleCircle[4][8].setChessNextCircle(getChessCircle(13,17),getChessCircle(12,16),getChessCircle(12,15), getChessCircle(13,15), nullptr, nullptr);
            triangleCircle[4][7].setChessNextCircle(getChessCircle(13,17),getChessCircle(13,16),getChessCircle(12,15), getChessCircle(11,15), nullptr, nullptr);
            triangleCircle[4][6].setChessNextCircle(getChessCircle(13,16),getChessCircle(12,15),getChessCircle(12,14),getChessCircle(13,14), nullptr, nullptr);

            triangleCircle[4][5].setChessNextCircle(getChessCircle(13,16),getChessCircle(12,16),getChessCircle(11,15), getChessCircle(11,14), getChessCircle(12,14), getChessCircle(13,15));
            triangleCircle[4][4].setChessNextCircle(getChessCircle(12,16),getChessCircle(12,15),getChessCircle(11,14), getChessCircle(10,14),nullptr, nullptr);
            triangleCircle[4][3].setChessNextCircle(getChessCircle(13,15),getChessCircle(12,14),gethonecomb(12,13),gethonecomb(13,13),nullptr, nullptr);

            triangleCircle[4][2].setChessNextCircle(getChessCircle(13,15),getChessCircle(12,15),getChessCircle(11,14), getChessCircle(13,14),gethonecomb(11,13),gethonecomb(12,13));
            triangleCircle[4][1].setChessNextCircle(getChessCircle(11,15),getChessCircle(12,15),getChessCircle(10,14),getChessCircle(12,14), gethonecomb(11,13),gethonecomb(10,13));

            triangleCircle[4][0].setChessNextCircle(getChessCircle(11,15),getChessCircle(11,14),gethonecomb(10,13),gethonecomb(9,13),nullptr, nullptr);

        // BLUE:
            for(int i=0; i< 10; i++){
                triangleCircle[1][i].setcolor(BLUE);
            }
            triangleCircle[1][0].setpos(1,5);
            triangleCircle[1][0].setFlag(true);

            triangleCircle[1][1].setpos(2,5);
            triangleCircle[1][1].setFlag(true);

            triangleCircle[1][2].setpos(2,6);
            triangleCircle[1][2].setFlag(true);

            triangleCircle[1][3].setpos(3,5);
            triangleCircle[1][3].setFlag(true);

            triangleCircle[1][4].setpos(3,6);
            triangleCircle[1][4].setFlag(true);

            triangleCircle[1][5].setpos(3,7);
            triangleCircle[1][5].setFlag(true);

            triangleCircle[1][6].setpos(4,5);
            triangleCircle[1][6].setFlag(true);

            triangleCircle[1][7].setpos(4,6);
            triangleCircle[1][7].setFlag(true);

            triangleCircle[1][8].setpos(4,7);
            triangleCircle[1][8].setFlag(true);

            triangleCircle[1][9].setpos(4,8);
            triangleCircle[1][9].setFlag(true);

            //空缺连接位置设置
            triangleCircle[1][0].setChessNextCircle(getChessCircle(2,5),getChessCircle(2,6),nullptr, nullptr, nullptr, nullptr);
            triangleCircle[1][1].setChessNextCircle(getChessCircle(1,5),getChessCircle(2,6),getChessCircle(3,6), getChessCircle(3,5), nullptr, nullptr);
            triangleCircle[1][2].setChessNextCircle(getChessCircle(1,5),getChessCircle(2,5),getChessCircle(3,6), getChessCircle(3,7), nullptr, nullptr);
            triangleCircle[1][3].setChessNextCircle(getChessCircle(2,5),getChessCircle(3,6),getChessCircle(4,6),getChessCircle(4,5), nullptr, nullptr);

            triangleCircle[1][4].setChessNextCircle(getChessCircle(2,5),getChessCircle(2,6),getChessCircle(3,5), getChessCircle(3,7), getChessCircle(4,7), getChessCircle(4,6));
            triangleCircle[1][5].setChessNextCircle(getChessCircle(2,6),getChessCircle(3,6),getChessCircle(4,7), getChessCircle(4,8),nullptr, nullptr);
            triangleCircle[1][6].setChessNextCircle(getChessCircle(3,5),getChessCircle(4,6),gethonecomb(5,6),gethonecomb(5,5),nullptr, nullptr);

            triangleCircle[1][7].setChessNextCircle(getChessCircle(4,5),getChessCircle(3,5),getChessCircle(3,6), getChessCircle(4,7),gethonecomb(5,7),gethonecomb(5,6));
            triangleCircle[1][8].setChessNextCircle(getChessCircle(4,6),getChessCircle(3,6),getChessCircle(3,7),getChessCircle(4,8), gethonecomb(5,7),gethonecomb(5,8));

            triangleCircle[1][9].setChessNextCircle(getChessCircle(3,7),getChessCircle(4,7),gethonecomb(5,8),gethonecomb(5,9),nullptr, nullptr);

        // ROSERED:
            for(int i=0; i< 10; i++){
                triangleCircle[5][i].setcolor(ROSERED);
            }
            triangleCircle[5][0].setpos(14,10);
            triangleCircle[5][0].setFlag(true);

            triangleCircle[5][1].setpos(14,11);
            triangleCircle[5][1].setFlag(true);

            triangleCircle[5][2].setpos(14,12);
            triangleCircle[5][2].setFlag(true);

            triangleCircle[5][3].setpos(14,13);
            triangleCircle[5][3].setFlag(true);

            triangleCircle[5][4].setpos(15,11);
            triangleCircle[5][4].setFlag(true);

            triangleCircle[5][5].setpos(15,12);
            triangleCircle[5][5].setFlag(true);

            triangleCircle[5][6].setpos(15,13);
            triangleCircle[5][6].setFlag(true);

            triangleCircle[5][7].setpos(16,12);
            triangleCircle[5][7].setFlag(true);

            triangleCircle[5][8].setpos(16,13);
            triangleCircle[5][8].setFlag(true);

            triangleCircle[5][9].setpos(17,13);
            triangleCircle[5][9].setFlag(true);

            //空缺连接位置设置
            triangleCircle[5][9].setChessNextCircle(getChessCircle(16,12),getChessCircle(16,13),nullptr, nullptr, nullptr, nullptr);
            triangleCircle[5][8].setChessNextCircle(getChessCircle(17,13),getChessCircle(16,12),getChessCircle(15,12), getChessCircle(15,13), nullptr, nullptr);
            triangleCircle[5][7].setChessNextCircle(getChessCircle(17,13),getChessCircle(16,13),getChessCircle(15,12), getChessCircle(15,11), nullptr, nullptr);
            triangleCircle[5][6].setChessNextCircle(getChessCircle(16,13),getChessCircle(15,12),getChessCircle(14,12),getChessCircle(14,13), nullptr, nullptr);

            triangleCircle[5][5].setChessNextCircle(getChessCircle(16,13),getChessCircle(16,12),getChessCircle(15,11), getChessCircle(14,11), getChessCircle(14,12), getChessCircle(15,13));
            triangleCircle[5][4].setChessNextCircle(getChessCircle(16,12),getChessCircle(15,12),getChessCircle(14,11), getChessCircle(14,10),nullptr, nullptr);
            triangleCircle[5][3].setChessNextCircle(getChessCircle(15,13),getChessCircle(14,12),gethonecomb(13,12),gethonecomb(13,13),nullptr, nullptr);

            triangleCircle[5][2].setChessNextCircle(getChessCircle(14,13),getChessCircle(15,13),getChessCircle(15,12), getChessCircle(14,11),gethonecomb(13,12),gethonecomb(13,11));
            triangleCircle[5][1].setChessNextCircle(getChessCircle(14,12),getChessCircle(15,12),getChessCircle(15,11),getChessCircle(14,10), gethonecomb(13,10),gethonecomb(13,11));

            triangleCircle[5][0].setChessNextCircle(getChessCircle(15,11),getChessCircle(14,11),gethonecomb(13,10),gethonecomb(13,9),nullptr, nullptr);


        // WHITE:
            for(int i=0; i< 10; i++){
                triangleCircle[2][i].setcolor(WHITE);
            }
            triangleCircle[2][0].setpos(5,10);
            triangleCircle[2][0].setFlag(true);

            triangleCircle[2][1].setpos(5,11);
            triangleCircle[2][1].setFlag(true);

            triangleCircle[2][2].setpos(5,12);
            triangleCircle[2][2].setFlag(true);

            triangleCircle[2][3].setpos(5,13);
            triangleCircle[2][3].setFlag(true);

            triangleCircle[2][4].setpos(6,11);
            triangleCircle[2][4].setFlag(true);

            triangleCircle[2][5].setpos(6,12);
            triangleCircle[2][5].setFlag(true);

            triangleCircle[2][6].setpos(6,13);
            triangleCircle[2][6].setFlag(true);

            triangleCircle[2][7].setpos(7,12);
            triangleCircle[2][7].setFlag(true);

            triangleCircle[2][8].setpos(7,13);
            triangleCircle[2][8].setFlag(true);

            triangleCircle[2][9].setpos(8,13);
            triangleCircle[2][9].setFlag(true);

            //空缺连接位置设置
            triangleCircle[2][3].setChessNextCircle(getChessCircle(5,12),getChessCircle(6,13),nullptr, nullptr, nullptr, nullptr);
            triangleCircle[2][2].setChessNextCircle(getChessCircle(5,13),getChessCircle(6,13),getChessCircle(6,12), getChessCircle(5,11), nullptr, nullptr);
            triangleCircle[2][6].setChessNextCircle(getChessCircle(5,13),getChessCircle(5,12),getChessCircle(6,12), getChessCircle(7,13), nullptr, nullptr);
            triangleCircle[2][1].setChessNextCircle(getChessCircle(5,12),getChessCircle(6,12),getChessCircle(6,11),getChessCircle(5,10), nullptr, nullptr);

            triangleCircle[2][5].setChessNextCircle(getChessCircle(6,13),getChessCircle(5,12),getChessCircle(5,11), getChessCircle(7,13), getChessCircle(7,12), getChessCircle(6,11));
            triangleCircle[2][8].setChessNextCircle(getChessCircle(6,13),getChessCircle(6,12),getChessCircle(7,12), getChessCircle(8,13),nullptr, nullptr);
            triangleCircle[2][0].setChessNextCircle(getChessCircle(5,11),getChessCircle(6,11),gethonecomb(6,10),gethonecomb(5,9),nullptr, nullptr);

            triangleCircle[2][4].setChessNextCircle(getChessCircle(5,10),getChessCircle(5,11),getChessCircle(6,12), getChessCircle(7,12),gethonecomb(7,11),gethonecomb(6,10));
            triangleCircle[2][7].setChessNextCircle(getChessCircle(8,13),getChessCircle(7,13),getChessCircle(6,12),getChessCircle(6,11), gethonecomb(7,11),gethonecomb(8,12));

            triangleCircle[2][9].setChessNextCircle(getChessCircle(7,13),getChessCircle(7,12),gethonecomb(8,12),gethonecomb(9,13),nullptr, nullptr);

        // GREEN:
            for(int i=0; i< 10; i++){
                triangleCircle[3][i].setcolor(GREEN);
            }
            triangleCircle[3][0].setpos(13,5);
            triangleCircle[3][0].setFlag(true);

            triangleCircle[3][1].setpos(12,5);
            triangleCircle[3][1].setFlag(true);

            triangleCircle[3][2].setpos(11,5);
            triangleCircle[3][2].setFlag(true);

            triangleCircle[3][3].setpos(10,5);
            triangleCircle[3][3].setFlag(true);

            triangleCircle[3][4].setpos(13,6);
            triangleCircle[3][4].setFlag(true);

            triangleCircle[3][5].setpos(12,6);
            triangleCircle[3][5].setFlag(true);

            triangleCircle[3][6].setpos(11,6);
            triangleCircle[3][6].setFlag(true);

            triangleCircle[3][7].setpos(13,7);
            triangleCircle[3][7].setFlag(true);

            triangleCircle[3][8].setpos(12,7);
            triangleCircle[3][8].setFlag(true);

            triangleCircle[3][9].setpos(13,8);
            triangleCircle[3][9].setFlag(true);

            //空缺连接位置设置
            triangleCircle[3][0].setChessNextCircle(getChessCircle(12,5),getChessCircle(13,6),nullptr, nullptr, nullptr, nullptr);
            triangleCircle[3][1].setChessNextCircle(getChessCircle(13,5),getChessCircle(13,6),getChessCircle(12,6), getChessCircle(11,5), nullptr, nullptr);
            triangleCircle[3][4].setChessNextCircle(getChessCircle(13,5),getChessCircle(12,5),getChessCircle(12,6), getChessCircle(13,7), nullptr, nullptr);
            triangleCircle[3][2].setChessNextCircle(getChessCircle(12,5),getChessCircle(12,6),getChessCircle(11,6),getChessCircle(10,5), nullptr, nullptr);

            triangleCircle[3][5].setChessNextCircle(getChessCircle(12,5),getChessCircle(13,6),getChessCircle(11,5), getChessCircle(13,7), getChessCircle(12,7), getChessCircle(11,6));
            triangleCircle[3][7].setChessNextCircle(getChessCircle(13,6),getChessCircle(12,6),getChessCircle(12,7), getChessCircle(13,8),nullptr, nullptr);
            triangleCircle[3][3].setChessNextCircle(getChessCircle(11,5),getChessCircle(11,6),gethonecomb(10,6),gethonecomb(9,5),nullptr, nullptr);

            triangleCircle[3][6].setChessNextCircle(getChessCircle(10,5),getChessCircle(11,5),getChessCircle(12,6), getChessCircle(12,7),gethonecomb(11,7),gethonecomb(10,6));
            triangleCircle[3][8].setChessNextCircle(getChessCircle(12,6),getChessCircle(11,6),getChessCircle(13,7),getChessCircle(13,8), gethonecomb(12,8),gethonecomb(11,7));

            triangleCircle[3][9].setChessNextCircle(getChessCircle(13,7),getChessCircle(12,7),gethonecomb(12,8),gethonecomb(13,9),nullptr, nullptr);

}
void CheckersTriangle::init(ChessColor chesscolor){
    if(chesscolor == SHARELIGHTGREEN)
        return ;

    usedflag = true;
    for(int i=0;i<10;i++){
        triangleChess[i].setcolor(chesscolor);
    }
    switch (chesscolor){
        case RED://10个棋子和空位的初始化
            triangleChess[0].setpos(5,1);//红色在设定的坐标系中，Y方向第1个，X方向第5行
            triangleChess[1].setpos(5,2);//红色在设定的坐标系中，Y方向第2个，X方向第5行
            triangleChess[2].setpos(5,3);//红色在设定的坐标系中，Y方向第3个，X方向第5行
            triangleChess[3].setpos(5,4);//红色在设定的坐标系中，Y方向第4个，X方向第5行
            triangleChess[4].setpos(6,2);//红色在设定的坐标系中，Y方向第2个，X方向第6行
            triangleChess[5].setpos(6,3);//红色在设定的坐标系中，Y方向第3个，X方向第6行
            triangleChess[6].setpos(6,4);//红色在设定的坐标系中，Y方向第4个，X方向第6行
            triangleChess[7].setpos(7,3);
            triangleChess[8].setpos(7,4);
            triangleChess[9].setpos(8,4);
            break;
        case ORANGE:
            triangleChess[0].setpos(10,14);
            triangleChess[1].setpos(11,14);
            triangleChess[2].setpos(12,14);
            triangleChess[3].setpos(13,14);
            triangleChess[4].setpos(11,15);
            triangleChess[5].setpos(12,15);
            triangleChess[6].setpos(13,15);
            triangleChess[7].setpos(12,16);
            triangleChess[8].setpos(13,16);
            triangleChess[9].setpos(13,17);

            break;
        case BLUE:
            triangleChess[0].setpos(1,5);
            triangleChess[1].setpos(2,5);
            triangleChess[2].setpos(2,6);
            triangleChess[3].setpos(3,5);
            triangleChess[4].setpos(3,6);
            triangleChess[5].setpos(3,7);
            triangleChess[6].setpos(4,5);
            triangleChess[7].setpos(4,6);
            triangleChess[8].setpos(4,7);
            triangleChess[9].setpos(4,8);

            break;
        case ROSERED:
            triangleChess[0].setpos(14,10);
            triangleChess[1].setpos(14,11);
            triangleChess[2].setpos(14,12);
            triangleChess[3].setpos(14,13);
            triangleChess[4].setpos(15,11);
            triangleChess[5].setpos(15,12);
            triangleChess[6].setpos(15,13);
            triangleChess[7].setpos(16,12);
            triangleChess[8].setpos(16,13);
            triangleChess[9].setpos(17,13);
            break;
        case WHITE:
            triangleChess[0].setpos(5,10);
            triangleChess[1].setpos(5,11);
            triangleChess[2].setpos(5,12);
            triangleChess[3].setpos(5,13);
            triangleChess[4].setpos(6,11);
            triangleChess[5].setpos(6,12);
            triangleChess[6].setpos(6,13);
            triangleChess[7].setpos(7,12);
            triangleChess[8].setpos(7,13);
            triangleChess[9].setpos(8,13);

            break;
        case GREEN:
            triangleChess[0].setpos(13,5);
            triangleChess[1].setpos(12,5);
            triangleChess[2].setpos(11,5);
            triangleChess[3].setpos(10,5);
            triangleChess[4].setpos(13,6);
            triangleChess[5].setpos(12,6);
            triangleChess[6].setpos(11,6);
            triangleChess[7].setpos(13,7);
            triangleChess[8].setpos(12,7);
            triangleChess[9].setpos(13,8);
            break;
        default:
            break;
    }

    //初始化所有不是订单的空缺位置ChessCircle

}

void CheckersMap::initMap() {

        for(int i =0; i < MAX_X; i++){
            for(int j =0; j < MAX_Y; j++){

                //没有棋子和不是空缺的位置，就是不能跳到的地方初始-1
                checkersmap[i][j] = NOCHESS;

                //跳起能够跳到的地方初始 SHARELIGHTGREEN
                //+1是为了标记位置是从1开始的
                if(chesschecker.IsLegalPosition(i+1,j+1)){
                    checkersmap[i][j] = SHARELIGHTGREEN;
                }
            }

            //有棋子的地方初始化为1，2，3，4，5，6,7
            //enum ChessColor{ RED=1, BLUE=2, WHITE=3, GREEN=4,ORANGE=5,ROSERED=6,SHARELIGHTGREEN=0,NOCHESS=-1};
            //每个棋子不同的颜色，初始不同的数字

            switch (context->getType()){
                int x ,y;
                case THREE:
                    for(int k=0; k< 6;k++){
                        for(int l=1; l< 11;l++){
                            x = trinangle[k].getChess(l).getx();
                            y = trinangle[k].getChess(l).gety();
                            //此处x y 各自减去 1的原因是，x y是从1 到17的一个坐标值，checkersmap是从0到16的一个数组
                            checkersmap[x-1][y-1] = trinangle[k].getChess(l).getcolor();
                        }
                    }
                    break;
                case TWO:
                    for(int k=0;k<4;k++){
                        for(int l=1; l< 11;l++){
                            x = trinangle[k].getChess(l).getx();
                            y = trinangle[k].getChess(l).gety();
                            checkersmap[x-1][y-1] = trinangle[k].getChess(l).getcolor();
                        }
                    }
                    break;
                case ONE:
                    for(int k=0;k<2;k++){
                        for(int l=1; l< 11;l++){
                            x = trinangle[k].getChess(l).getx();
                            y = trinangle[k].getChess(l).gety();
                            checkersmap[x-1][y-1] = trinangle[k].getChess(l).getcolor();
                        }
                    }
                    break;
                default:
                    break;
            }
    }
}
void CheckersMap::printmap() {
    for(int i=0; i< MAX_X; i++) {
        for (int j = 0; j < MAX_Y; j++) {
            if(checkersmap[i][j] != -1 )
                printf(" %d,", checkersmap[i][j]);
            else
                printf("%d,", checkersmap[i][j]);
        }
        printf("\n");
    }
}
void ChessCircle::setChessNextCircle(      ChessCircle *nextChessCircle0,
                                           ChessCircle *nextChessCircle1,
                                           ChessCircle *nextChessCircle2,
                                           ChessCircle *nextChessCircle3,
                                           ChessCircle *nextChessCircle4,
                                           ChessCircle *nextChessCircle5) {

}
int main(int,char *argv[])
{
    CheckersMap* jumper = new CheckersMap();

    namedWindow(WINDOW_NAME_CHESS);
    CheckersUI ui;
    ui.Triangle();
    ui.DrawBackground();
    ui.DrawButton(1);
    //ui.printChess(ui.getMapXY(6,3),ui.getMapXY(7,7),RED);
    //ui.printChess(ui.getMapXY(7,7),ui.getMapXY(8,9),RED);
    //ui.printChess(ui.getMapXY(13,17),ui.getMapXY(16,13),ORANGE);
    Mat bg = ui.getmat();
    imshow(WINDOW_NAME_CHESS,bg);

    CheckersImageObject ob;

    //ob.findChessColor_v2(RED);
    //ob.findChessColor(BLUE);


    waitKey();
    return 1;
}