//
// Created by litm on 2020/3/14.
//

#include "MouseMoveChessOpt.h"

void MouseMoveChessOpt::DrawRectangle(Mat img, Rect box,bool flag) {
    if(flag)
        rectangle(img,box.tl(),box.br(),Scalar(g_rng.uniform(0,255),g_rng.uniform(0,255),g_rng.uniform(0,255)));
    else
        rectangle(img,box.tl(),box.br(),Scalar(0,0,0));
}
void MouseMoveChessOpt::on_MouseHandle(int event, int x, int y, int flags, void *param){

    switch(event){
        case EVENT_MOUSEMOVE:
        {

        }
        break;
        case EVENT_LBUTTONUP:
        {

        }
        break;
        case EVENT_LBUTTONDOWN:
        {
            curMousePos.x = x;
            curMousePos.y = y;
        }
        break;
    }
}
void MouseMoveChessOpt::on_MouseHandle_test(int event, int x, int y, int flags, void *param) {
    Mat image = *(Mat*)param;
    curMousePos.x = x;
    curMousePos.y = y;
    switch(event){
        case EVENT_MOUSEMOVE:
        {
            g_old_rectangle = g_rectangle;
            if(g_bDrawingBox){
                g_rectangle.width = x - g_rectangle.x;
                g_rectangle.height = y - g_rectangle.y;
            }
            if(g_rectangle.width <0){
                g_rectangle.x += g_rectangle.width;
                g_rectangle.width *= -1;
            }

            if(g_rectangle.height <0){
                g_rectangle.y += g_rectangle.height;
                g_rectangle.height *=-1;
            }
            Mat tempImage;
            DrawRectangle(image,g_old_rectangle,false);
            DrawRectangle(image,g_rectangle);
            image.copyTo(tempImage);
            imshow("Mouse Test",tempImage);
        }
        break;

        case EVENT_LBUTTONDOWN:
        {
            g_bDrawingBox = true;
            g_rectangle = Rect(x,y,0,0);
        }
            break;

        case EVENT_LBUTTONUP:
        {
            g_bDrawingBox = false;
            if(g_rectangle.width <0){
                g_rectangle.x += g_rectangle.width;
                g_rectangle.width *= -1;
            }

            if(g_rectangle.height <0){
                g_rectangle.y += g_rectangle.height;
                g_rectangle.height *=-1;
            }
            Mat tempImage;
            DrawRectangle(image,g_rectangle);
            image.copyTo(tempImage);
            imshow("Mouse Test",tempImage);
        }
            break;
    }
}
void MouseMoveChessOpt::showWindows_test() {

    g_rectangle = Rect(-1,-1,0,0);
    Mat srcImage(600,800,CV_8UC3),tempImage;
    srcImage.copyTo(tempImage);
    g_rectangle = Rect(-1,-1,0,0);
    srcImage = Scalar::all(0);

    namedWindow("Mouse Test");


    WrapMouseOpt optwap;
    optwap.optthis = this;
    optwap.image = srcImage;
    setMouseCallback("Mouse Test",onMouseHandleGlobal,(void*)&optwap);

    while(1){
        srcImage.copyTo(tempImage);
        if(g_bDrawingBox)
            DrawRectangle(tempImage,g_rectangle);

        imshow("Mouse Test",tempImage);
        if(waitKey(0) == 27)break;
    }

    return ;

}