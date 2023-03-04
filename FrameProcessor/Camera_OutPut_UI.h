//
// Created by litm on 2023/2/5.
//

#ifndef YOLO_CAMERA_OUTPUT_UI_H
#define YOLO_CAMERA_OUTPUT_UI_H
#include "../ChessInclude.h"
#include "../yolovx/yolo.h"
#include "Chessboardinfo.h"
#include "../CheckersMapLimitCheck.h"
#include<algorithm>
#include<vector>
#include<iostream>
#include<math.h>
#include "../utils/inifile.h"
//#include "../CheckersUI.h"

using namespace inifile;
class SaveDetectResultInfo{
public:
    // logical world or memory chess board info
    int index;
    int x;
    int y;
    // physical wold chess board info
    Point circle_center;
    int radius;
public:
    SaveDetectResultInfo(){
        index = -1;
        x =0;
        y =0;
        circle_center.x =0;
        circle_center.y =0;
        radius =0;
    }
    SaveDetectResultInfo(int index,int x, int y,Point center,int radius =19){
        this->index = index;
        this->x = x;
        this->y = y;
        this->circle_center = center;
        this->radius = radius;
    }


};
struct DetectResult_UI{
    Point circle_center;
    int radius;
    Output orgDectRet;
    DetectChessThreeinfo info;
public:
    DetectResult_UI(Point cent,int radius,Output dbox,int redfirst_index,int redfirst_x,int redfirst_y,ChessColor color){
        circle_center = cent;
        this->radius = radius;
        orgDectRet = dbox;
        info.setInfo(redfirst_index,redfirst_x,redfirst_y,color);
    }
    DetectResult_UI(Output& dbox,DetectChessThreeinfo& dectInfo,int radius_input = 19){
        orgDectRet = dbox;
        info = dectInfo;
        CalculateRectCenterPosion(dbox.box,circle_center);
        radius = radius_input;
    }
    DetectResult_UI(){
        radius = -1;
    }
    void setDectUI(Output dbox,DetectChessThreeinfo dectInfo,int radius_input = 19){
        orgDectRet = dbox;
        info = dectInfo;
        CalculateRectCenterPosion(dbox.box,circle_center);
        radius = radius_input;
    }

};
class mycomp_x {
public:
    bool operator() (const Output & x1,const Output & x2 ){
        Point cent1,cent2;
        CalculateRectCenterPosion(x1.box,cent1);
        CalculateRectCenterPosion(x2.box,cent2);
        return (cent1.x < cent2.x);
    }
};
class mycomp_y {
public:
    bool operator() (const Output & y1,const Output & y2 ) {
        Point cent1, cent2;
        CalculateRectCenterPosion(y1.box, cent1);
        CalculateRectCenterPosion(y2.box, cent2);
        return (cent1.y < cent2.y);
    }
};
class mycomp_map_y {
public:
    bool operator() (pair<int,vector<Output>> m1,pair<int,vector<Output>> m2){
        if(m1.second.size() <=0 ||
           m2.second.size() <=0)
            return false;
        Rect L1 = m1.second.begin()->box;
        Rect L2 = m2.second.begin()->box;
        Point cent1,cent2;
        CalculateRectCenterPosion(L1, cent1);
        CalculateRectCenterPosion(L2, cent2);
        return (cent1.y < cent2.y);
    }
};
class Camera_OutPut_UI {
    Mat button_check_yolov5;
    Mat ui_output_mat;
    vector<Output> result_current;
    vector<Output> result_current_no_chess;
    Point iB,iG,iO,iR,iR2,iW;
    Rect iB_R,iG_R,iO_R,iR_R,iR2_R,iW_R;
    const int TOP_X_2_CENTER_DISCTANCE=19;
    const int CHESS_CIRCLE_DISCTANCE_MAX=38;
    const int ADJUST_PARAM=30;
    const int ADJUST_LEFT_RIGHT=10;
    //Chessboardinfo chessboard_info_camera;
    CheckersMapLimitCheck checker;

    map<int,DetectResult_UI> detectRetMap;//record every once detect result
    map<int,SaveDetectResultInfo> SaveDetectPhysical2LogicalInfo;

    //
    bool temp_debug_flag= false;
    bool adjust_flag= true;

    //
    //CheckersUI *ui;
    map<int,list<CircleReturn>> *MapChessControlMemory;
    IniFile inifp;
    //
    //had save
    bool had_save_flag = false;
private:
    void CalculateResult();
    void CalculateHadSaveFromConfigParseResult();
    void DrawDetectChessOntoBoard();
    static bool comp_map(pair<int,vector<Output>> m1,pair<int,vector<Output>> m2){
        Rect L1 = m1.second.begin()->box;
        Rect L2 = m2.second.begin()->box;
        Point cent1,cent2;
        CalculateRectCenterPosion(L1, cent1);
        CalculateRectCenterPosion(L2, cent2);
        return (cent1.y > cent2.y);
    }
public:
    Camera_OutPut_UI(){
        button_check_yolov5 = imread("../image/button_begin.png",IMREAD_UNCHANGED);
    }
    Camera_OutPut_UI(Mat output){
        button_check_yolov5 = imread("../image/button_begin.png",IMREAD_UNCHANGED);
        ui_output_mat = output;
        //chessboard_info_camera.setMat(ui_output_mat);
    }
    void setOutputMat(Mat output){
        ui_output_mat = output;
        //chessboard_info_camera.setMat(ui_output_mat);
    }
    Mat getOutputMat(){
        return ui_output_mat;
    }
    void setDetectResult(vector<Output>& result){
        result_current.clear();
        for (int i = 0; i < result.size(); i++){
            result_current.push_back(result[i]);
        }
    }
    void setMemMap(map<int,list<CircleReturn>>* map){
        MapChessControlMemory = map;
    }
    void setDetectResult_no_chess(vector<Output>& result){
        result_current_no_chess.clear();
        for (int i = 0; i < result.size(); i++){
            result_current_no_chess.push_back(result[i]);
        }
    }
    //only call this function on init detect
    int CalculateDetectResult();

    void CalculateDetectResult_Demo();
    void DrawBackground();
    void DrawButton(int type);
    int getJumpChessCircle(int x ,int y,Point& circle_center,int& radius);

    int getJumpChessCircle(Rect input);
    int getJumpChessCircle(Mat input);
    Mat getImgRect(Rect one,Rect two,Rect& output);
    Mat getMask(Rect one, Rect two);
    void rotate(Mat input,Mat& output,vector<Output> result);
    void rotate(cv::Mat src, cv::Mat& dst, double angle);

    //
    void parseDetectMapToMemory();
    void parseDetectChessPositionToBoardUpdateFromSaveResult( vector<Output>& output,vector<Output>& output_nochess);
    void setSaveFlag(bool flag){
        had_save_flag = flag;
    }
    //
    void ReUpdateCheckersUIMapChessControlMemory();
};


#endif //YOLO_CAMERA_OUTPUT_UI_H
