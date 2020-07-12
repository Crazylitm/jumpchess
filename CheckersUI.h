//
// Created by litm on 2020/2/2.
//

#ifndef JUMP0_CHECKERSUI_H
#define JUMP0_CHECKERSUI_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "CheckersMapLimitCheck.h"
#include "ChinessJumpChessControl.h"
#include <list>
#include <map>
#include "ChessNode.h"
using namespace cv;
using  namespace std;
const int  WINDOW_WIDTH = 1200;
const int  WINDOW_HIGHT = 900;
const int  RED_TOP_5_1_X = WINDOW_WIDTH/2;
const int  RED_TOP_5_1_Y = 30;
class CheckersUI {
private:
    Mat chessmapmat;
    Mat chessmapmat_no_chess_org;
    Point BLUE_TOP_1_5_point;
    Point RED_TOP_5_1_point;
    Point GREEN_TOP_13_5_point;
    Point ROSERED_TOP_17_13_point;
    Point ORANGE_TOP_13_17_point;
    Point WHIRTE_TOP_5_13_point;
    int begx=RED_TOP_5_1_X;
    int begy=RED_TOP_5_1_Y;
    int  xwidth=30;// x方向的每个棋子圆心的距离
    int  yhigh=52;//y方向每个棋子圆心的距离
    Mat redchess;
    Mat bluechess;
    Mat greenchess;
    Mat roseredchess;
    Mat whirtechess;
    Mat orangechess;

    Point curMousePos;//鼠标点击的位置


    ChinessJumpChessControl *control= nullptr;
    ChessNode *head = nullptr;
public:
    CheckersMapLimitCheck checker;
    map<int,list<CircleReturn>> MapChessControlMemory;
public:
    CheckersUI(){
        chessmapmat = Mat(WINDOW_HIGHT,WINDOW_WIDTH,CV_8UC3,Scalar(255,255,255));
        RED_TOP_5_1_point = Point(RED_TOP_5_1_X,RED_TOP_5_1_Y);
        ROSERED_TOP_17_13_point = Point(begx-xwidth*12,begy+yhigh*12);
        WHIRTE_TOP_5_13_point = Point(begx+xwidth*12,begy+yhigh*12);
        ORANGE_TOP_13_17_point = Point(begx,begy+yhigh*16);
        GREEN_TOP_13_5_point = Point(begx-xwidth*12,begy+yhigh*4);
        BLUE_TOP_1_5_point =Point(begx+xwidth*12,begy+yhigh*4);
        redchess = imread("../image/redchess.png",IMREAD_UNCHANGED);
        bluechess = imread("../image/bluechess.png",IMREAD_UNCHANGED);
        greenchess = imread("../image/greenchess.png",IMREAD_UNCHANGED);
        roseredchess = imread("../image/roseredchess.png",IMREAD_UNCHANGED);
        whirtechess = imread("../image/writechess.png",IMREAD_UNCHANGED);
        orangechess = imread("../image/orangechess.png",IMREAD_UNCHANGED);

        control = new ChinessJumpChessControl(this->chessmapmat,&this->checker,&MapChessControlMemory);
    }
    CheckersUI(ChinessJumpChessControl *p){
        control = p;

        chessmapmat = Mat(WINDOW_HIGHT,WINDOW_WIDTH,CV_8UC3,Scalar(255,255,255));
        RED_TOP_5_1_point = Point(RED_TOP_5_1_X,RED_TOP_5_1_Y);
        ROSERED_TOP_17_13_point = Point(begx-xwidth*12,begy+yhigh*12);
        WHIRTE_TOP_5_13_point = Point(begx+xwidth*12,begy+yhigh*12);
        ORANGE_TOP_13_17_point = Point(begx,begy+yhigh*16);
        GREEN_TOP_13_5_point = Point(begx-xwidth*12,begy+yhigh*4);
        BLUE_TOP_1_5_point =Point(begx+xwidth*12,begy+yhigh*4);
        redchess = imread("../image/redchess.png",IMREAD_UNCHANGED);
        bluechess = imread("../image/bluechess.png",IMREAD_UNCHANGED);
        greenchess = imread("../image/greenchess.png",IMREAD_UNCHANGED);
        roseredchess = imread("../image/roseredchess.png",IMREAD_UNCHANGED);
        whirtechess = imread("../image/writechess.png",IMREAD_UNCHANGED);
        orangechess = imread("../image/orangechess.png",IMREAD_UNCHANGED);
    }
    ~CheckersUI(){
        delete control;
    }
    Mat getmat(){
        return chessmapmat;
    }
    Mat getOrgmat(){
        return chessmapmat_no_chess_org;
    }
    void DrawFiledCircle(Mat img,Point center,int radius=CHESS_RADIUS);
    void DrawEllipse(Mat img,double angle);
    void DrawLine(Mat img,Point start,Point end);
    void Line(int x1,int y1,int x2,int y2);
    void Circle(int x,int y,int radius=CHESS_RADIUS);
    void Circle(Point p,int radius=CHESS_RADIUS);
    void Circle(Point center,int Type, int radius=CHESS_RADIUS);//Type={0:虚线圆 选中状态 1:}
    void Circle(list<Point> *p,int Type, int radius=CHESS_RADIUS);
    void Triangle(Point p1,Point p2,Point P3,ChessColor color);
    void Triangle();
    void InitChess();
    Point getMapXY(int x ,int y);//通过逻辑棋盘坐标的x，y获得实际的画图UI的x，y坐标
    void printChess(Point org,Point dist,ChessColor chess);
    int cvAdd4cMat_q(cv::Mat &dst, cv::Mat &scr, double scale);

    void initMouseParam();
    void onMouseHandle_inner(int event,int x, int y ,int flags,void* param);

    void initMapList();
    void initChessNodeGraph();

    CircleReturn* getCirclePosXY(int x, int y);//通过鼠标位置点击xy位置，获得当前选中的物体，如果没有Point返回-（-1，-1）
    void updateCircleMap(int CircleMap_i,Point p, ChessColor color);//鼠标点击移动棋子后在更新内存最新的棋子的位置和颜色
    float getDistance(Point pointO, Point pointA)
    {
        float distance;
        distance = powf((pointO.x - pointA.x), 2) + powf((pointO.y - pointA.y), 2);
        distance = sqrtf(distance);
        return distance;
    }
    void printChessMap();
};

static void onMouseHandle(int event,int x, int y ,int flags,void* param){
    CheckersUI *p = (CheckersUI*)param;
    p->onMouseHandle_inner(event,x,y,flags,param);
}

#endif //JUMP0_CHECKERSUI_H
