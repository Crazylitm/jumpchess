//
// Created by litm on 2023/2/5.
//

#include "Chessboardinfo.h"

/*
bool Chessboardinfo::save_detect_result_ini() {

    if(detectRetMap.empty())
        return false;



    return true;
}

bool Chessboardinfo::read_detect_result_ini() {

    return true;
}
void Chessboardinfo::parseDetectMapToMemory() {
    if(detectRetMap.empty())
        return;
    map<int,DetectResult_UI>::iterator iter = detectRetMap.begin();

    int cur_x,cur_y;
    ChessColor cur_color;
    int i=0;
    map<int,list<CircleReturn>> head = ui->MapChessControlMemory;
    while(iter != detectRetMap.end()){

        map<int,list<CircleReturn>>::iterator  iterfind = head.find(iter->first);
        if(iterfind != head.end())
        {
            iterfind->second.begin()->CircleMap_i = iter->second.info.index;
            iterfind->second.begin()->Map_x = iter->second.info.x;
            iterfind->second.begin()->Map_y = iter->second.info.y;
            iterfind->second.begin()->curColor= iter->second.info.chessColor;
            iterfind->second.begin()->curPoint = iter->second.circle_center;
            if(iter->second.info.chessColor == ChessColor(NOCHESS))
                iterfind->second.begin()->Down_Chess_flag = true;
            else
                iterfind->second.begin()->Down_Chess_flag = false;
        }
        iter++;
        i++;
    }
}
 */