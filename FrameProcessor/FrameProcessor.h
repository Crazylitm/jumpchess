//
// Created by litm on 2020/7/18.
//

#ifndef JUMP0_FRAMEPROCESSOR_H
#define JUMP0_FRAMEPROCESSOR_H

#include "../ChessInclude.h"

class FrameProcessor {
public:
    virtual void process(Mat &input,Mat &output)=0 ;
    virtual void init()=0;
    virtual void setMemMap(map<int,list<CircleReturn>>* map)=0;
    virtual void setUpdateMemfp(void(*fpun)())=0;
    virtual void setThreshold(int id)=0;
};


#endif //JUMP0_FRAMEPROCESSOR_H
