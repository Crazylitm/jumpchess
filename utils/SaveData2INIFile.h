//
// Created by litm on 2023/3/12.
//

#ifndef YOLO_SAVEDATA2INIFILE_H
#define YOLO_SAVEDATA2INIFILE_H
#include "inifile.h"
#include "../FrameProcessor/Camera_OutPut_UI.h"
using namespace inifile;

#define S_SECTION "CHESS PARAMETER  "
#define S_INDEX "index"
#define S_X "x"
#define S_Y "y"
#define S_POINT_X "point.x"
#define S_POINT_Y "point.y"
#define S_RADIUS "radius"

class SaveData2INIFile {

    IniFile inifp;
public:
    SaveData2INIFile(){
        init("./chessboard_detect_data.ini");
    };
    SaveData2INIFile(string filename){
        init(filename);
    }
    ~SaveData2INIFile(){

    }
    void init(string filename ){
        inifp.iniFilePath = filename;
    };
    int GetiniFile(vector<SaveDetectResultInfo>& saveRet);
    int GetiniFile(string filename,vector<SaveDetectResultInfo>& saveRet);
    void SaveiniFile(vector<SaveDetectResultInfo>& saveRet);
    void SaveiniFile(string filename,vector<SaveDetectResultInfo>& saveRet);


};


#endif //YOLO_SAVEDATA2INIFILE_H
