//
// Created by litm on 2023/3/12.
//

#include "SaveData2INIFile.h"
#include <cstdlib>

// The ini file is external input: values must be range-checked before they are
// used as CircleMap indexes / board coordinates. strtol instead of atoi so
// non-numeric and overflowing text is rejected instead of silently truncated.
static bool parseBoundedInt(const string &s, long minv, long maxv, int *out) {
    char *end = nullptr;
    long v = strtol(s.c_str(), &end, 10);
    if (end == s.c_str() || v < minv || v > maxv) {
        return false;
    }
    *out = (int)v;
    return true;
}

static bool parseDetectRecord(const string &index_s, const string &x_s, const string &y_s,
                              const string &point_x_s, const string &point_y_s,
                              const string &radius_s, SaveDetectResultInfo *out) {
    int idx, x, y, px, py, radius;
    if (!parseBoundedInt(index_s, 0, MAX_CHESS - 1, &idx) ||
        !parseBoundedInt(x_s, 0, 32, &x) ||
        !parseBoundedInt(y_s, 0, 32, &y) ||
        !parseBoundedInt(point_x_s, -100000, 100000, &px) ||
        !parseBoundedInt(point_y_s, -100000, 100000, &py) ||
        !parseBoundedInt(radius_s, 0, 100000, &radius)) {
        return false;
    }
    *out = SaveDetectResultInfo(idx, x, y, Point(px, py), radius);
    return true;
}

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

        SaveDetectResultInfo info;
        if(!parseDetectRecord(index_s,x_s,y_s,point_x_s,point_y_s,radius_s,&info)){
            cout << "skip invalid record in ini section [" << section << "]" << endl;
            continue;
        }
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

        SaveDetectResultInfo info;
        if(!parseDetectRecord(index_s,x_s,y_s,point_x_s,point_y_s,radius_s,&info)){
            cout << "skip invalid record in ini section [" << section << "]" << endl;
            continue;
        }
        saveRet.push_back(info);
    }
    return 0 ;
}