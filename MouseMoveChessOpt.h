//
// Created by litm on 2020/3/14.
//

#ifndef JUMP0_MOUSEMOVECHESSOPT_H
#define JUMP0_MOUSEMOVECHESSOPT_H

#include "ChessInclude.h"
using namespace cv;
using namespace std;


class MouseMoveChessOpt {
public:
    void on_MouseHandle(int event,int x, int y ,int flags,void* param);
    void on_MouseHandle_test(int event,int x, int y ,int flags,void* param);
    void DrawRectangle(Mat img, Rect box,bool flag= true);
    void ShowHelpText();
    void showWindows_test();

    MouseMoveChessOpt(){
        curMousePos.x =0;
        curMousePos.y =0;
        g_bDrawingBox = false;
    }

public:
     Rect g_rectangle,g_old_rectangle;
     bool g_bDrawingBox;
     RNG g_rng;
     Point curMousePos;


};
struct WrapMouseOpt{
    MouseMoveChessOpt *optthis;
    Mat image;
};
static void onMouseHandleGlobal(int event,int x, int y ,int flags,void* param){
    WrapMouseOpt *opt = (WrapMouseOpt*) param;
    opt->optthis->on_MouseHandle(event,x,y,flags,&opt->image);
}

#endif //JUMP0_MOUSEMOVECHESSOPT_H
