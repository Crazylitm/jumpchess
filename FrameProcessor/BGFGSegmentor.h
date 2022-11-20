//
// Created by litm on 2020/7/18.
//

#ifndef JUMP0_BGFGSEGMENTOR_H
#define JUMP0_BGFGSEGMENTOR_H


#include "FrameProcessor.h"
struct lines_node{
    float rho;
    float theta;
    Point p1;
    Point p2;
    int count;
    int jd_type;//0-90 = 1; 90-180 =2 ; 180-270 3 ;270 -360 =4;
    float distance_2_center;
    bool max_flag;
    lines_node(){
        rho =0;
        theta =0;
        count=0;
        jd_type=-1;
        distance_2_center=0;
        max_flag =false;
    };
};
class BGFGSegmentor : public FrameProcessor{
     Mat gray;
     Mat background;
     Mat backImage;
     Mat foreground;
     double learningRate;
     int threshold;
     Point Max_B_center;
     int Max_B_radius;
     long fnumber=0;
     float  Max_B_theta;
     Mat in_fun,out_fun;
     Rect Max_Rect;
public:
     BGFGSegmentor(){
         Max_B_center = Point(-1,-1);
         Max_B_radius = -1;
     }
     void process(Mat &input,Mat &output);
     void process_v1(Mat &input,Mat &output);
     void process_v2(Mat &input,Mat &output);//将跳棋棋盘从背景中扣出来，方便处理
     void process_v3(Mat &input,Mat &output);
     void LocatChessCircle(Mat &input,Mat&output);
     void setThreshold(int id){
         threshold = id;
     }
    double get_point_angle(Point pointO,Point pointA);
     void DrawChessStandLines(Mat &input ,Mat &output);

};


#endif //JUMP0_BGFGSEGMENTOR_H
