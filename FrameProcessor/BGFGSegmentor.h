//
// Created by litm on 2020/7/18.
//

#ifndef JUMP0_BGFGSEGMENTOR_H
#define JUMP0_BGFGSEGMENTOR_H


#include "FrameProcessor.h"
#include <opencv2//opencv.hpp>
#include <math.h>
#include <opencv2/core.hpp>
#include "../yolovx/yolo.h"
#include "FrameProcessor.h"
#include "Camera_OutPut_UI.h"
#include "../UI.h"
#include "../utils/SaveData2INIFile.h"

using namespace std;
using namespace cv;
using namespace dnn;
#define WINDOW_NAME_CHESS_OUTPUT "Chess Capture Output"


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
class BGFGSegmentor : public FrameProcessor,UI{
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
     //add Yolo
     Yolo test;
     Net net;
     Net net_no_ches;
     vector<Scalar> color;

     //add 2023.2.5
     Camera_OutPut_UI outPutUi;
     int ButtonTypeStatus=1;

     //
     UI *pui= nullptr;

     //had save
     bool had_save_flag = false;
     bool had_UI_update_no = false;
     SaveData2INIFile fpfile;
public:
     BGFGSegmentor(){
         Max_B_center = Point(-1,-1);
         Max_B_radius = -1;
         //add Yolo
         //python export.py --weights /home/litm/soft/yolov5/runs/train/exp22/weights/best.pt --img 640 --include onnx
         string model_path = "/home/litm/soft/yolov5/runs/train/exp24/weights/best_status.onnx";
         if (test.readModel(net, model_path, false)) {
             cout << "read net ok!" << endl;
         }
         else {
             return;
         }
         string model_path_detect_no_chess = "/home/litm/soft/yolov5/runs/train/exp35/weights/best.onnx";
         if (test.readModel(net_no_ches, model_path_detect_no_chess, false)) {
             cout << "read net ok!" << endl;
         }
         else {
             return;
         }
         color.push_back((Scalar(255,118,20)));//Blue
         color.push_back((Scalar(68,163,16)));//Green
         color.push_back((Scalar(16,93,255)));//Orange
         color.push_back((Scalar(0,0,255)));//Red
         color.push_back((Scalar(255,79,255)));//RoseRed
         color.push_back((Scalar(255,255,255)));//White
     }
     void process(Mat &input,Mat &output);
     void process_v1(Mat &input,Mat &output);
     void process_v2(Mat &input,Mat &output);//将跳棋棋盘从背景中扣出来，方便处理
     void process_v3(Mat &input,Mat &output);
     void LocatChessCircle(Mat &input,Mat&output);
     void setThreshold(int id){
         threshold = id;
     }
    void setMemMap(map<int,list<CircleReturn>>* map){
        outPutUi.setMemMap(map);
    }
    void setUpdateMemfp(void(*fpun)()){

     }
    double get_point_angle(Point pointO,Point pointA);
     void DrawChessStandLines(Mat &input ,Mat &output);

    void initMouseParam();
    void onMouseHandle_BGFGSegmentor_inner(int event,int x, int y ,int flags,void* param);

    void init();

    void rotate(cv::Mat src, cv::Mat& dst, double angle);
    void update(){
         if(pui== nullptr)
             return;
         pui->update();
    }
    void setUI(UI* ui){
        pui = ui;
    }
    bool judgeLastoneAndThisoneEqual(vector<Output>& lastone,vector<Output>& thisone);

    //
    bool ReadIniToSaveInfo();

};
static void onMouseHandle_BGFGSegmentor_imShow(int event,int x, int y ,int flags,void* param){
    BGFGSegmentor *p = (BGFGSegmentor*)param;
    // printf("onMouseHandle_Camera_imShow\n") ;
    p->onMouseHandle_BGFGSegmentor_inner(event,x,y,flags,param);
}

#endif //JUMP0_BGFGSEGMENTOR_H
