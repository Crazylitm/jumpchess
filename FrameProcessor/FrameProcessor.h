//
// Created by litm on 2020/7/18.
//

#ifndef JUMP0_FRAMEPROCESSOR_H
#define JUMP0_FRAMEPROCESSOR_H

#include "../ChessInclude.h"

class FrameProcessor {
public:
    virtual void process(Mat &input,Mat &output){} ;
};


#endif //JUMP0_FRAMEPROCESSOR_H
