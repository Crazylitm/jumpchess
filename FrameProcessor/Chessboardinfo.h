//
// Created by litm on 2023/2/5.
//

#ifndef YOLO_CHESSBOARDINFO_H
#define YOLO_CHESSBOARDINFO_H
#include "../ChessInclude.h"
#include "Camera_OutPut_UI.h"
using namespace std;
using namespace cv;
using namespace dnn;
//from camera to get chess board info class
class Chessboardinfo {
/*
    Mat board_ui_mat;
    map<int,DetectResult_UI> detectRetMap;
    CheckersMapLimitCheck checker;
    CheckersUI *ui;
    IniFile inifp;
    */
public:
    Chessboardinfo(){

    }
    /*
    Chessboardinfo(Mat output){
        board_ui_mat = output;
    }
    Chessboardinfo(Mat output,map<int,DetectResult_UI> dMap){
         board_ui_mat = output;
         detectRetMap = dMap;
    }
    Chessboardinfo(Mat output,map<int,DetectResult_UI> dMap,CheckerUI inui){
        board_ui_mat = output;
        detectRetMap = dMap;
        ui = inui;
    }
    void setUI(CheckerUI& inui){
        ui = inui;

    }
    void setMat(Mat output){
        board_ui_mat = output;
    }
    void setResultMap(map<int,DetectResult_UI> dMap){
        detectRetMap = dMap;
    }
    bool save_detect_result_ini();
    bool read_detect_result_ini();
    void parseDetectMapToMemory();
    */
};


#endif //YOLO_CHESSBOARDINFO_H
