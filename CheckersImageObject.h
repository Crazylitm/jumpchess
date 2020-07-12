//
// Created by litm on 2020/2/8.
//

#ifndef JUMP0_CHECKERSIMAGEOBJECT_H
#define JUMP0_CHECKERSIMAGEOBJECT_H

#include "ChessInclude.h"
using namespace cv;
using namespace std;
class CheckersImageObject {
private:
    Mat orgimage;
public:
    CheckersImageObject(){

    }
    void readImage();
    void test2();
    void calcHistNow();
    void findChessColor(ChessColor color=RED);
    void findChessColor_v2(ChessColor color=RED);
    //void onChangeTrackBar (int pos,void* usrdata);
};



#endif //JUMP0_CHECKERSIMAGEOBJECT_H
