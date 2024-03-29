//
// Created by litm on 2020/7/16.
//

#ifndef JUMP0_CHESSCAMERA_H
#define JUMP0_CHESSCAMERA_H

#include "ChessInclude.h"
#include "FrameProcessor/FrameProcessor.h"
//#include "FrameProcessor/BGFGSegmentor.h"
#define WINDOW_NAME_CHESS_OUTPUT "Chess Capture Output"
#define WINDOW_NAME_CHESS_INPUT "Chess Capture Input"
class ChessCamera {
    FrameProcessor* frameProcessor;
    //OpenCV视频捕获对象
    VideoCapture capture;
    // 处理每一帧时都会调用的回调函数
    void (*process)(Mat&,Mat&);
    // 回调函数是否被调用
    bool callIt;
    //输入窗口的显示名称
    string windowNameInput="Chess Capture Input";
    string windowsNameDealIn="Video Deal in Win";
    //输出窗口的显示名称
    string windowNameOutput="Chess Capture Output";
    //帧之间的延时
    int delay;
    //已经处理的帧数
    long fnumber;
    //达到这个帧数时结束
    long frameToStop;
    //结束处理
    bool stop;

    Mat camera_matrix = Mat(3, 3, CV_32FC1);
    Mat distortion_coefficients;
    //Mat output_mat_windows;
public:
   void init(map<int,list<CircleReturn>>* map,FrameProcessor& segmentor);
   void init_vtest();
   void setFrameProcessor(void(*frameProcessingCallback)(Mat&,Mat&)){
     process = frameProcessingCallback;
   }
   void setFrameProcessor(FrameProcessor* frameProcessorPtr){
       process =0;
       frameProcessor = frameProcessorPtr;
       frameProcessorPtr->init();
       callProcess();
   }
   bool setInput(int id){
       fnumber =0;
       capture.release();
       stopIt();
       return capture.open(id,CAP_V4L);
   }
   bool isOpened(){
       return capture.isOpened();
   }
   void stopIt(){
       stop = true;
   }
   bool isStopped(){
       return stop;
   }
   void setDelay(int d){
       delay = d;
   }
   bool readNextFrame(Mat& frame){
       return capture.read(frame);
   }
   void displayInput(string wn){
       windowNameInput = wn;
       //destroyWindow(windowNameInput);
       //namedWindow(windowNameInput);
   }
   void displayOutput(string wn){
       windowNameOutput = wn;
       //destroyWindow(windowNameOutput);
       namedWindow(windowNameOutput);
   }
   void run();
   void callProcess(){
       callIt = true;
   }
   void dontCallProcess(){
       callIt = false;
   }
   void stopAtFrameNo(long frame){
       frameToStop = frame;
   }
   long getFrameNumber(){
       long fnumber = static_cast<long>(capture.get(CAP_PROP_POS_FRAMES));
       return fnumber;
   }
   void DoUndistort(Mat &src,Mat &out);
    void setMemMap(map<int,list<CircleReturn>>* map){
        frameProcessor->setMemMap(map);
    }
    void setUpdateMemfp(void(*fpun)()){
        frameProcessor->setUpdateMemfp(fpun);
    }
};

#endif //JUMP0_CHESSCAMERA_H
