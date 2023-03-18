//
// Created by litm on 2023/3/12.
//

#include "SaveData2INIFile.h"

void SaveData2INIFile::SaveiniFile(vector<SaveDetectResultInfo> &saveRet) {
    vector<SaveDetectResultInfo>::iterator iter;
    for(iter = saveRet.begin();iter != saveRet.end(); iter++){
        string section = S_SECTION + to_string(iter->index);
        inifp.setValue(section,S_INDEX,to_string(iter->index)," chess position num");
        inifp.setValue(section,S_X, to_string(iter->x),"logical position [x] ");
        inifp.setValue(section,S_Y, to_string(iter->y),"logical position [y]");
        inifp.setValue(section,S_POINT_X, to_string(iter->circle_center.x),"physical position [x] of circle center");
        inifp.setValue(section,S_POINT_Y, to_string(iter->circle_center.y),"physical position [y] of circle center");
        inifp.setValue(section,S_RADIUS, to_string(iter->radius),"physical radius of circle");
    }
    inifp.Save();
}
void SaveData2INIFile::SaveiniFile(string filename, vector<SaveDetectResultInfo> &saveRet) {
    vector<SaveDetectResultInfo>::iterator iter;
    for(iter = saveRet.begin();iter != saveRet.end(); iter++){
        string section = S_SECTION + to_string(iter->index);
        inifp.setValue(section,S_INDEX,to_string(iter->index)," chess position num");
        inifp.setValue(section,S_X, to_string(iter->x),"logical position [x] ");
        inifp.setValue(section,S_Y, to_string(iter->y),"logical position [y]");
        inifp.setValue(section,S_POINT_X, to_string(iter->circle_center.x),"physical position [x] of circle center");
        inifp.setValue(section,S_POINT_Y, to_string(iter->circle_center.y),"physical position [y] of circle center");
        inifp.setValue(section,S_RADIUS, to_string(iter->radius),"physical radius of circle");
    }
    inifp.SaveAs(filename);
}
int SaveData2INIFile::GetiniFile(vector<SaveDetectResultInfo> &saveRet) {
    if(inifp.Load(inifp.iniFilePath) != RET_OK){
        return -1;
    }
    int ini_size = inifp.GetSectionNum() - 1;
    if(ini_size <1)
        return -1;
    saveRet.clear();
    for(int i=0; i < ini_size; i++){
        string section = S_SECTION + to_string(i);
        string index_s;
        int iRet = inifp.getValue(section,S_INDEX,&index_s);
        if( iRet != RET_OK){
            cout << inifp.errMsg << endl;
            return -1;
        }
        string x_s;
        iRet = inifp.getValue(section,S_X,&x_s);
        if( iRet != RET_OK){
            cout << inifp.errMsg << endl;
            return -1;
        }
        string y_s;
        iRet = inifp.getValue(section,S_Y,&y_s);
        if( iRet != RET_OK){
            cout << inifp.errMsg << endl;
            return -1;
        }
        string point_x_s;
        iRet = inifp.getValue(section,S_POINT_X,&point_x_s);
        if( iRet != RET_OK){
            cout << inifp.errMsg << endl;
            return -1;
        }
        string point_y_s;
        iRet = inifp.getValue(section,S_POINT_Y,&point_y_s);
        if( iRet != RET_OK){
            cout << inifp.errMsg << endl;
            break;
        }
        string radius_s;
        iRet = inifp.getValue(section,S_RADIUS,&radius_s);
        if( iRet != RET_OK){
            cout << inifp.errMsg << endl;
            return -1;
        }

        SaveDetectResultInfo info(atoi(index_s.c_str()),atoi(x_s.c_str()),atoi(y_s.c_str()),
                                  Point(atoi(point_x_s.c_str()),atoi(point_y_s.c_str())),atoi(radius_s.c_str()));
        saveRet.push_back(info);
    }
    return 0 ;
}

int SaveData2INIFile::GetiniFile(string filename, vector<SaveDetectResultInfo> &saveRet) {
    if(inifp.Load(filename) != RET_OK){
        return -1;
    }
    int ini_size = inifp.GetSectionNum();
    if(ini_size <1)
        return -1;
    saveRet.clear();
    for(int i=0; i < ini_size; i++){
        string section = S_SECTION + to_string(i+1) + " ]";
        string index_s;
        int iRet = inifp.getValue(section,S_INDEX,&index_s);
        if( iRet != RET_OK){
            cout << inifp.errMsg << endl;
            return -1;
        }
        string x_s;
        iRet = inifp.getValue(section,S_X,&x_s);
        if( iRet != RET_OK){
            cout << inifp.errMsg << endl;
            return -1;
        }
        string y_s;
        iRet = inifp.getValue(section,S_Y,&y_s);
        if( iRet != RET_OK){
            cout << inifp.errMsg << endl;
            return -1;
        }
        string point_x_s;
        iRet = inifp.getValue(section,S_POINT_X,&point_x_s);
        if( iRet != RET_OK){
            cout << inifp.errMsg << endl;
            return -1;
        }
        string point_y_s;
        iRet = inifp.getValue(section,S_POINT_Y,&point_y_s);
        if( iRet != RET_OK){
            cout << inifp.errMsg << endl;
            break;
        }
        string radius_s;
        iRet = inifp.getValue(section,S_RADIUS,&radius_s);
        if( iRet != RET_OK){
            cout << inifp.errMsg << endl;
            return -1;
        }

        SaveDetectResultInfo info(atoi(index_s.c_str()),atoi(x_s.c_str()),atoi(y_s.c_str()),
                                  Point(atoi(point_x_s.c_str()),atoi(point_y_s.c_str())),atoi(radius_s.c_str()));
        saveRet.push_back(info);
    }
    return 0 ;
}