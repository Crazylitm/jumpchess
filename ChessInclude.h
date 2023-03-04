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
#include "yolovx/yolo.h"



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
const int MAX_JUMP_CHESS=60;//
//定义棋子的颜色，6个角，有6种颜色
enum ChessColor{ RED=1, BLUE=2, WHITE=3, GREEN=4,ORANGE=5,ROSERED=6,SHARELIGHTGREEN=0,NOCHESS=-1};

enum NextType{SELF=0,TLEFT=1,TRIGHT=2,MLEFT=3,MRIGHT=4,DLEFT=5,DRIGHT=6};
//定义空位置的颜色，除了6个角和棋子颜色一样外，中间还有一块浅绿色的共享区域的颜色
//enum ChessCircleColor{ ,SHARELIGHTGREEN};
const int MAX_X=17;
const int MAX_Y=17;

typedef void(*Fpfun)();
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
//png带4通道的图像才能把透明背景显示出来
//https://www.cnblogs.com/huty/p/8517372.html
//函数的逻辑就是把scr的4通道和dst的3通道融合在一起，让4通道里面的透明通道能够和dst完美融合
static bool cvAdd4cMat_q_static(cv::Mat &dst, cv::Mat &scr, double scale)
{
    int dstc= dst.channels();
    int scrc= scr.channels();
    if ( dstc!= 3 ||  scrc!= 4)
    {
        return true;
    }
    if (scale < 0.01)
        return false;
    std::vector<cv::Mat>scr_channels;
    std::vector<cv::Mat>dstt_channels;
    split(scr, scr_channels);
    split(dst, dstt_channels);
    CV_Assert(scr_channels.size() == 4 && dstt_channels.size() == 3);

    if (scale < 1)
    {
        scr_channels[3] *= scale;
        scale = 1;
    }
    for (int i = 0; i < 3; i++)
    {
        dstt_channels[i] = dstt_channels[i].mul(255.0 / scale - scr_channels[3], scale / 255.0);
        dstt_channels[i] += scr_channels[i].mul(scr_channels[3], scale / 255.0);
    }
    merge(dstt_channels, dst);
    return true;
}
static bool CalculateRectCenterPosion(Rect box,Point& cent){
    if(box.x <0 || box.y <0 || box.height <=0 || box.width <=0)
        return false;
    cent.x = (box.tl().x+box.br().x)/2;
    cent.y = (box.tl().y+box.br().y)/2;
    return true;
}
static bool rectA_intersect_rectB(cv::Rect rectA, cv::Rect rectB)
{
    if ( rectA.x > rectB.x + rectB.width ) { return false; }
    if ( rectA.y > rectB.y + rectB.height ) { return false; }
    if ( (rectA.x + rectA.width) < rectB.x ) { return false; }
    if ( (rectA.y + rectA.height) < rectB.y ) { return false; }

    float colInt =  min(rectA.x+rectA.width,rectB.x+rectB.width) - max(rectA.x, rectB.x);
    float rowInt =  min(rectA.y+rectA.height,rectB.y+rectB.height) - max(rectA.y,rectB.y);
    float intersection = colInt * rowInt;
    float areaA = rectA.width * rectA.height;
    float areaB = rectB.width * rectB.height;
    float intersectionPercent =  intersection / (areaA + areaB - intersection);

    if ( (0 < intersectionPercent)&&(intersectionPercent < 1)&&(intersection != areaA)&&(intersection != areaB) )
    {
        return true;
    }

    return false;
}
static bool Calculate2PointCentP(Point one,Point two,Point& cent){
    if(one.x <0 || one.y <0 || two.x <=0 || two.y <=0)
        return false;
    int x_len = abs(one.x - two.x);
    int y_len = abs(one.y - two.y);
    if(one.x < two.x)
    {
        if(one.y < two.y){

            cent.x = one.x+x_len/2;
            cent.y = one.y+y_len/2;
        }else if(one.y > two.y){
            cent.x = one.x+x_len/2;
            cent.y = two.y+y_len/2;
        }else{
            //===
            cent.x = one.x+x_len/2;
            cent.y = two.y;
        }

    } else if (one.x > two.x)
    {
        if(one.y < two.y){
            cent.x = two.x+x_len/2;
            cent.y = one.y+y_len/2;
        }else if(one.y > two.y){
            cent.x = two.x+x_len/2;
            cent.y = two.y+y_len/2;
        }else{
            //===
            cent.x = two.x+x_len/2;
            cent.y = two.y;
        }
    } else{
        //===
        if(one.y < two.y){
            cent.x = two.x;
            cent.y = one.y+y_len/2;
        }else if(one.y > two.y){
            cent.x = two.x;
            cent.y = two.y+y_len/2;
        }else{
            //===
            cent.x = two.x;
            cent.y = two.y;
        }
    }
    return true;
}
static float getDistance(Point pointO, Point pointA)
{
    float distance;
    distance = powf((pointO.x - pointA.x), 2) + powf((pointO.y - pointA.y), 2);
    distance = sqrtf(distance);
    return distance;
}
static std::map<int, std::set<int>> generateBorders(const std::vector<cv::Point> & vecPts)
{
    std::map<int, std::set<int>> borders;
    cv::Point tl(vecPts[0]), tr(vecPts[1]), bl(vecPts[2]), br(vecPts[3]);

    // tl to tr
    double K = double(tl.y-tr.y) / (tl.x - tr.x);
    for (int i = tl.x; i < tr.x; ++i)
        borders[i].insert(cvRound(-K*(tl.x - i) + tl.y));

    // tr to br
    if (tr.x != br.x)
    {
        K = double(tr.y - br.y) / (tr.x - br.x);
        for (int i = tr.x; i < br.x; ++i)
            borders[i].insert(cvRound(-K*(tr.x - i) + tr.y));
    }

    // br to bl
    K = double(br.y - bl.y) / (br.x - bl.x);
    for (int i = bl.x; i < br.x; ++i)
        borders[i].insert(cvRound(-K*(br.x - i) + br.y));

    // bl to tl
    if (bl.x != tl.x)
    {
        K = double(bl.y - tl.y) / (bl.x - tl.x);
        for (int i = tl.x; i < bl.x; ++i)
            borders[i].insert(cvRound(-K*(bl.x - i) + bl.y));
    }

    for (auto it = borders.begin(); it!=borders.end(); ++it)
    {
        if ((*it).second.size() == 2)
            continue;

        std::set<int> newone = { *(*it).second.begin(), *(--(*it).second.end())};
        (*it).second.swap(newone);
    }

#ifdef DEBUG_CODE
    cv::Mat disp = cv::imread("/home/litm/git_code/jumpchess/IMG_4015.JPG" );

	for (auto it = borders.begin(); it != borders.end(); ++it)
	{
		cv::circle(disp, cv::Point((*it).first, *(*it).second.begin()), 1, cv::Scalar(255, 255, 0), -1);
		cv::circle(disp, cv::Point((*it).first, *(--(*it).second.end())), 1, cv::Scalar(0, 255, 255), -1);

	}

	cv::circle(disp, tl, 2, cv::Scalar(0, 0, 255), -1);
	cv::circle(disp, tr, 2, cv::Scalar(0, 0, 255), -1);
	cv::circle(disp, bl, 2, cv::Scalar(0, 0, 255), -1);
	cv::circle(disp, br, 2, cv::Scalar(0, 0, 255), -1);

    cv::namedWindow("Debug4");
    cv::imshow("Debug4",disp);
    cv::waitKey();
#endif // DEBUG_CODE
    return borders;
}

static cv::Mat generateMask(const cv::Mat & src,
                            const std::vector<cv::Point> & vecPts)
{
    cv::Mat mask = cv::Mat::zeros(src.size(), CV_8UC1);

    cv::Point tl(vecPts[0]), tr(vecPts[1]), bl(vecPts[2]), br(vecPts[3]);
    std::map<int, std::set<int>> borders = generateBorders(vecPts);

    int minX = std::min(tl.x, bl.x),
            maxX = std::max(tr.x, br.x),
            minY = std::min(tl.y, tr.y),
            maxY = std::max(bl.y, br.y);

    uchar minZ = std::min(src.at<uchar>(tl), src.at<uchar>(tr));
    minZ = std::min(minZ, src.at<uchar>(bl));
    minZ = std::min(minZ, src.at<uchar>(br));

    for (size_t j= minY; j<maxY; ++j)
    {
        const uchar* pS = src.ptr<uchar>(j);
        uchar* pM = mask.ptr<uchar>(j);
        for (size_t i = minX; i < maxX; ++i)
        {
            // in the region.
            if (*borders[i].begin() < j && j < *(++borders[i].begin()))
            {
                pM[i] = pS[i];
            }
        }
    }

    return mask;

}
//enum ChessColor{ RED=1, BLUE=2, WHITE=3, GREEN=4,ORANGE=5,ROSERED=6,SHARELIGHTGREEN=0,NOCHESS=-1}; define Chessinclue.h;
//enum YoloChessColor{ Y_BLUE=0,Y_GREEN=1,Y_ORANGE=2,Y_RED=3,Y_ROSERED=4,Y_WHITE=5,Y_NOCHESS=-1};define Yolo.h
static ChessColor ConvertYoloColor2ChessColor(YoloChessColor one){
    switch(one){
        case Y_BLUE:
            return (ChessColor(BLUE));
            break;
        case Y_GREEN:
            return (ChessColor(GREEN));
            break;
        case Y_ORANGE:
            return (ChessColor(ORANGE));
            break;
        case Y_RED:
            return (ChessColor(RED));
            break;
        case Y_ROSERED:
            return (ChessColor(ROSERED));
            break;
        case Y_WHITE:
            return (ChessColor(WHITE));
            break;
        default:
            return (ChessColor(NOCHESS));
            //Y_NOCHESS
        break;
    }
}
#endif //JUMP0_CHESSINCLUDE_H
