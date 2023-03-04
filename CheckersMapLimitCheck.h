//
// Created by litm on 2020/2/2.
//

#ifndef JUMP0_CHECKERSMAPLIMITCHECK_H
#define JUMP0_CHECKERSMAPLIMITCHECK_H

#include "ChessInclude.h"

using  namespace std;
using namespace cv;
const int MAX_CELL=192;
const int MAX_CHESS=121;

struct DetectChessThreeinfo{
    int index;
    int x;
    int y;
    ChessColor chessColor;
public:
    DetectChessThreeinfo(int redfirst_index,int redfirst_x,int redfirst_y,ChessColor color){
        index = redfirst_index;
        x = redfirst_x;
        y = redfirst_y;
        chessColor = color;
    }
    DetectChessThreeinfo(){
        index = -1;
    }
    void setInfo(int redfirst_index,int redfirst_x,int redfirst_y,ChessColor color){
        index = redfirst_index;
        x = redfirst_x;
        y = redfirst_y;
        chessColor = color;
    }
};
class CheckersMapLimitCheck {
public:
    CheckersMapLimitCheck(){
        for(int i=0;i < MAX_CHESS ;i ++){
            int x = CircleMap[i][0];
            int y = CircleMap[i][1];
            int color1 = CircleMap[i][2];

            if(mappingChessPosition[y].size() <= 0){
                vector<DetectChessThreeinfo> info;
                info.push_back(DetectChessThreeinfo(i,x,y,ChessColor(color1)));
                mappingChessPosition[y] = info;
            }else{

                mappingChessPosition[y].push_back(DetectChessThreeinfo(i,x,y,ChessColor(color1)));
            }


        }
        for(int i=5;i<=13;i++){
            vector<DetectChessThreeinfo> info = mappingChessPosition[i];
            vector<DetectChessThreeinfo> info_save,info_new;
            for(int j=0;j<info.size();j++){
                if(info[j].chessColor != ChessColor(BLUE) &&
                        info[j].chessColor != ChessColor(WHITE) ){
                    info_new.push_back(info[j]);
                }else {
                    info_save.push_back(info[j]);
                }
            }
            for(int z=info_save.size()-1;z>=0;z--){
                info_new.push_back(info_save[z]);
            }
            mappingChessPosition[i] = info_new;
        }
    }
public:
    int  mapXCountToYMin2Max[MAX_X][2] {
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
public:
    map<int,vector<DetectChessThreeinfo>> mappingChessPosition;


    int CircleMap[MAX_CHESS][3]={
            /*
              top Blue : x first line , (1,5)
              top Red : y first line , (5,1)

            */
            //RED 10
            {5,1,RED},{6,2,RED},{5,2,RED},{7,3,RED},{6,3,RED},{5,3,RED},{8,4,RED},{7,4,RED},{6,4,RED},{5,4,RED},
            //GREEN 20
            {13,5,GREEN},{13,6,GREEN},{12,5,GREEN},{13,7,GREEN},{12,6,GREEN},{11,5,GREEN},{13,8,GREEN},{12,7,GREEN},{11,6,GREEN},{10,5,GREEN},
            //ROSERED 30
            {17,13,ROSERED},{16,13,ROSERED},{16,12,ROSERED},{15,13,ROSERED},{15,12,ROSERED},{15,11,ROSERED},
            {14,13,ROSERED},{14,12,ROSERED},{14,11,ROSERED},{14,10,ROSERED},
            //ORANGE 40
            {13,17,ORANGE},{13,16,ORANGE},{12,16,ORANGE},{13,15,ORANGE},{12,15,ORANGE},{11,15,ORANGE},
            {13,14,ORANGE},{12,14,ORANGE},{11,14,ORANGE},{10,14,ORANGE},
            //WHITE 50
            {5,13,WHITE},{5,12,WHITE},{6,13,WHITE},{5,11,WHITE},{6,12,WHITE},{7,13,WHITE},
            {5,10,WHITE},{6,11,WHITE},{7,12,WHITE},{8,13,WHITE},
            //BLUE 60
            {1,5,BLUE},{2,5,BLUE},{2,6,BLUE},{3,5,BLUE},{3,6,BLUE},{3,7,BLUE},{4,5,BLUE},{4,6,BLUE},{4,7,BLUE},{4,8,BLUE},
            //Honeycomb
            //1-5 65
            {9,5,SHARELIGHTGREEN},{8,5,SHARELIGHTGREEN},{7,5,SHARELIGHTGREEN},{6,5,SHARELIGHTGREEN},{5,5,SHARELIGHTGREEN},
            //2-6 71
            {10,6,SHARELIGHTGREEN},{9,6,SHARELIGHTGREEN},{8,6,SHARELIGHTGREEN},{7,6,SHARELIGHTGREEN},{6,6,SHARELIGHTGREEN},{5,6,SHARELIGHTGREEN},
            //3-7 78
            {11,7,SHARELIGHTGREEN},{10,7,SHARELIGHTGREEN},{9,7,SHARELIGHTGREEN},{8,7,SHARELIGHTGREEN},
            {7,7,SHARELIGHTGREEN},{6,7,SHARELIGHTGREEN},{5,7,SHARELIGHTGREEN},
            //4-8
            {12,8,SHARELIGHTGREEN},{11,8,SHARELIGHTGREEN},{10,8,SHARELIGHTGREEN},{9,8,SHARELIGHTGREEN},
            {8,8,SHARELIGHTGREEN},{7,8,SHARELIGHTGREEN},{6,8,SHARELIGHTGREEN},{5,8,SHARELIGHTGREEN},
            //5-9
            {13,9,SHARELIGHTGREEN},{12,9,SHARELIGHTGREEN},{11,9,SHARELIGHTGREEN},{10,9,SHARELIGHTGREEN},
            {9,9,SHARELIGHTGREEN},{8,9,SHARELIGHTGREEN},{7,9,SHARELIGHTGREEN},{6,9,SHARELIGHTGREEN},{5,9,SHARELIGHTGREEN},
            //6-8
            {13,10,SHARELIGHTGREEN},{12,10,SHARELIGHTGREEN},{11,10,SHARELIGHTGREEN},{10,10,SHARELIGHTGREEN},
            {9,10,SHARELIGHTGREEN},{8,10,SHARELIGHTGREEN},{7,10,SHARELIGHTGREEN},{6,10,SHARELIGHTGREEN},
            //7-7
            {13,11,SHARELIGHTGREEN},{12,11,SHARELIGHTGREEN},{11,11,SHARELIGHTGREEN},{10,11,SHARELIGHTGREEN},
            {9,11,SHARELIGHTGREEN},{8,11,SHARELIGHTGREEN},{7,11,SHARELIGHTGREEN},
            //8-6
            {13,12,SHARELIGHTGREEN},{12,12,SHARELIGHTGREEN},{11,12,SHARELIGHTGREEN},{10,12,SHARELIGHTGREEN},
            {9,12,SHARELIGHTGREEN},{8,12,SHARELIGHTGREEN},
            //9-5
            {13,13,SHARELIGHTGREEN},{12,13,SHARELIGHTGREEN},{11,13,SHARELIGHTGREEN},{10,13,SHARELIGHTGREEN},
            {9,13,SHARELIGHTGREEN}

    };
    //把整个棋盘看作是由192个三角型组成的，定义这些小三角形的关系
    int littleTriangleMap[MAX_CELL][3][2]={
            //RED
            {{5,1},{6,2},{5,2}},
            {{6,2},{7,3},{6,3}},
            {{6,2},{5,2},{6,3}},
            {{5,2},{6,3},{5,3}},
            {{7,3},{8,4},{7,4}},
            {{7,3},{7,4},{6,3}},
            {{6,3},{7,4},{6,4}},
            {{6,3},{6,4},{5,3}},
            {{5,3},{6,4},{5,4}},
            //RED EDGE
            {{8,4},{9,5},{8,5}},
            {{8,4},{8,5},{7,4}},
            {{7,4},{8,5},{7,5}},
            {{7,4},{6,4},{7,5}},
            {{6,4},{7,5},{6,5}},
            {{6,4},{6,5},{5,4}},
            {{5,4},{6,5},{5,5}},
            //BLUE
            {{1,5},{2,5},{2,6}},
            {{2,5},{3,5},{3,6}},
            {{2,5},{3,6},{2,6}},
            {{2,6},{3,6},{3,7}},
            {{3,5},{4,5},{4,6}},
            {{3,5},{4,6},{3,6}},
            {{3,6},{4,6},{4,7}},
            {{3,6},{4,7},{3,7}},
            {{3,7},{4,7},{4,8}},
            //BLUE EDGE
            {{4,5},{5,5},{5,6}},
            {{4,5},{5,6},{4,6}},
            {{4,6},{5,6},{5,7}},
            {{4,6},{5,7},{4,7}},
            {{4,7},{5,7},{5,8}},
            {{4,7},{5,8},{4,8}},
            {{4,8},{5,8},{5,9}},
            //WHITE
            {{5,13},{5,12},{6,13}},
            {{5,12},{5,11},{6,12}},
            {{5,12},{6,12},{6,13}},
            {{6,13},{6,12},{7,13}},
            {{5,11},{5,10},{6,11}},
            {{5,11},{6,11},{6,12}},
            {{6,12},{6,11},{7,12}},
            {{6,12},{7,12},{7,13}},
            {{7,13},{7,12},{8,13}},
            //WHITE EDGE
            {{5,10},{5,9},{6,10}},
            {{5,10},{6,10},{6,11}},
            {{6,11},{6,10},{7,11}},
            {{6,11},{7,11},{7,12}},
            {{7,12},{7,11},{8,12}},
            {{7,12},{8,12},{8,13}},
            {{8,13},{8,12},{9,13}},
            //ORANGE
            {{13,17},{12,16},{13,16}},
            {{12,16},{11,15},{12,15}},
            {{12,16},{12,15},{13,16}},
            {{13,16},{12,15},{13,15}},
            {{11,15},{10,14},{11,14}},
            {{11,15},{11,14},{12,15}},
            {{12,15},{11,14},{12,14}},
            {{12,15},{12,14},{13,15}},
            {{13,15},{12,14},{13,14}},
            //ORANGE EDGE
            {{10,14},{9,13},{10,13}},
            {{10,14},{10,13},{11,14}},
            {{11,14},{10,13},{11,13}},
            {{11,14},{11,13},{12,14}},
            {{12,14},{11,13},{12,13}},
            {{12,14},{12,13},{13,14}},
            {{13,14},{12,13},{13,13}},
            //ROSERRED
            {{17,13},{16,13},{16,12}},
            {{16,13},{15,13},{15,12}},
            {{16,13},{15,12},{16,12}},
            {{16,12},{15,12},{15,11}},
            {{15,13},{14,13},{14,12}},
            {{15,13},{14,12},{15,12}},
            {{15,12},{14,12},{14,11}},
            {{15,12},{14,11},{15,11}},
            {{15,11},{14,11},{14,10}},
            //ROSERED EDGE
            {{14,13},{13,13},{13,12}},
            {{14,13},{13,12},{14,12}},
            {{14,12},{13,12},{13,11}},
            {{14,12},{13,11},{14,11}},
            {{14,11},{13,11},{13,10}},
            {{14,11},{13,10},{14,10}},
            {{14,10},{13,10},{13,9}},
            //GREEN
            {{13,5},{12,5},{13,6}},
            {{13,6},{13,7},{12,6}},
            {{13,6},{12,6},{12,5}},
            {{12,5},{12,6},{11,5}},
            {{13,7},{13,8},{12,7}},
            {{13,7},{12,7},{12,6}},
            {{12,6},{12,7},{11,6}},
            {{12,6},{11,6},{11,5}},
            {{11,5},{11,6},{10,5}},
            //GREEN EDGE
            {{13,8},{13,9},{12,8}},
            {{13,8},{12,7},{12,8}},
            {{12,7},{12,8},{11,7}},
            {{12,7},{11,7},{11,6}},
            {{11,6},{11,7},{10,6}},
            {{11,6},{10,6},{10,5}},
            {{10,5},{10,6},{9,5}},
            //HONEYCOMB -1
            {{9,5},{10,6},{9,6}},
            {{9,5},{8,5},{9,6}},
            {{8,5},{9,6},{8,6}},
            {{8,5},{8,6},{7,5}},
            {{7,5},{8,6},{7,6}},
            {{7,5},{7,6},{6,5}},
            {{6,5},{7,6},{6,6}},
            {{6,5},{6,6},{5,5}},
            {{5,5},{6,6},{5,6}},
            //HONEYCOMB -2
            {{10,6},{11,7},{10,7}},
            {{10,6},{10,7},{9,6}},
            {{9,6},{10,7},{9,7}},
            {{9,6},{9,7},{8,6}},
            {{8,6},{9,7},{8,7}},
            {{8,6},{8,7},{7,6}},
            {{7,6},{8,7},{7,7}},
            {{7,6},{7,7},{6,6}},
            {{6,6},{7,7},{6,7}},
            {{6,6},{6,7},{5,6}},
            {{5,6},{6,7},{5,7}},
            //HONEYCOMB -3
            {{11,7},{12,8},{11,8}},
            {{11,7},{11,8},{10,7}},
            {{10,7},{11,8},{10,8}},
            {{10,7},{10,8},{9,7}},
            {{9,7},{10,8},{9,8}},
            {{9,7},{9,8},{8,7}},
            {{8,7},{9,8},{8,8}},
            {{8,7},{8,8},{7,7}},
            {{7,7},{8,8},{7,8}},
            {{7,7},{7,8},{6,7}},
            {{6,7},{7,8},{6,8}},
            {{6,7},{6,8},{5,7}},
            {{5,7},{6,8},{5,8}},
            //HONEYCOMB -4
            {{12,8},{13,9},{12,9}},
            {{12,8},{12,9},{11,8}},
            {{11,8},{12,9},{11,9}},
            {{11,8},{11,9},{10,8}},
            {{10,8},{11,9},{10,9}},
            {{10,8},{10,9},{9,8}},
            {{9,8},{10,9},{9,9}},
            {{9,8},{9,9},{8,8}},
            {{8,8},{9,9},{8,9}},
            {{8,8},{8,9},{7,8}},
            {{7,8},{8,9},{7,9}},
            {{7,8},{7,9},{6,8}},
            {{6,8},{7,9},{6,9}},
            {{6,8},{6,9},{5,8}},
            {{5,8},{6,9},{5,9}},
            //HONEYCOMB -5
            {{13,9},{12,9},{13,10}},
            {{12,9},{13,10},{12,10}},
            {{12,9},{12,10},{11,9}},
            {{11,9},{12,10},{11,10}},
            {{11,9},{11,10},{10,9}},
            {{10,9},{11,10},{10,10}},
            {{10,9},{10,10},{9,9}},
            {{9,9},{10,10},{9,10}},
            {{9,9},{9,10},{8,9}},
            {{8,9},{9,10},{8,10}},
            {{8,9},{8,10},{7,9}},
            {{7,9},{8,10},{7,10}},
            {{7,9},{7,10},{6,9}},
            {{6,9},{7,10},{6,10}},
            {{6,9},{6,10},{5,9}},
            //HONEYCOMB -6
            {{13,10},{13,11},{12,10}},
            {{12,10},{13,11},{12,11}},
            {{12,10},{12,11},{11,10}},
            {{11,10},{12,11},{11,11}},
            {{11,10},{11,11},{10,10}},
            {{10,10},{11,11},{10,11}},
            {{10,10},{10,11},{9,10}},
            {{9,10},{10,11},{9,11}},
            {{9,10},{9,11},{8,10}},
            {{8,10},{9,11},{8,11}},
            {{8,10},{8,11},{7,10}},
            {{7,10},{8,11},{7,11}},
            {{7,10},{7,11},{6,10}},
            //HONEYCOMB -7
            {{13,11},{13,12},{12,11}},
            {{12,11},{13,12},{12,12}},
            {{12,11},{12,12},{11,11}},
            {{11,11},{12,12},{11,12}},
            {{11,11},{11,12},{10,11}},
            {{10,11},{11,12},{10,12}},
            {{10,11},{10,12},{9,11}},
            {{9,11},{10,12},{9,12}},
            {{9,11},{9,12},{8,11}},
            {{8,11},{9,12},{8,12}},
            {{8,11},{8,12},{7,11}},
            //HONEYCOMB -8
            {{13,12},{12,12},{13,13}},
            {{12,12},{13,13},{12,13}},
            {{12,12},{12,13},{11,12}},
            {{11,12},{12,13},{11,13}},
            {{11,12},{11,13},{10,12}},
            {{10,12},{11,13},{10,13}},
            {{10,12},{10,13},{9,12}},
            {{9,12},{10,13},{9,13}},
            {{9,12},{9,13},{8,12}}
    };
public:

    bool IsLegalPosition(int x ,int y);

    int getDistanceFromRED_5_1_point_X(int x,int y);
    int getDistanceFromRED_5_1_point_Y(int x, int y);
    ChessColor getChessColor(int x,int y);
    ChessColor  getCircleColor(int x,int y);
    list<Point>* getChessNeighbourList(Point curnode);
    int getCircleMapPostion(int x,int y);

};
static CheckersMapLimitCheck chesschecker;
#endif //JUMP0_CHECKERSMAPLIMITCHECK_H
