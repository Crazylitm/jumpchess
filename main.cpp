#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void onChangeTrackBar (int pos,void* usrdata) {
    // 强制类型转换
    Mat src = *(Mat *) (usrdata);
    Mat hsv,out_image,out;
    int l_h,l_s,l_v,u_s,u_h,u_v;
    l_h = getTrackbarPos("L - H","Trackbars");
    l_s = getTrackbarPos("L - S","Trackbars");
    l_v = getTrackbarPos("L - V","Trackbars");
    u_h = getTrackbarPos("U - H","Trackbars");
    u_s = getTrackbarPos("U - S","Trackbars");
    u_v = getTrackbarPos("U - V","Trackbars");
    //定义灰度和显示灰度后图像
    cvtColor(src,hsv,COLOR_BGR2HSV);

    inRange(hsv,Scalar(l_h,l_s,l_v),Scalar(u_h,u_s,u_v),out_image);

    bitwise_and(hsv,hsv,out,out_image);

    imshow("Trackbars", out);
}


int main_(int,char *argv[])
{
    Mat in_image,hsv, out_image;

    int trackbas_l_h_min = 0;
    int trackbas_l_s_min = 0;
    int trackbas_l_v_min = 0;
    int trackbas_u_s_min = 179;
    int trackbas_u_h_min = 255;
    int trackbas_u_v_min = 255;

    int l_h,l_s,l_v,u_s,u_h,u_v;

    //读取图片
    in_image = imread(argv[1], IMREAD_UNCHANGED);

    if(in_image.empty()){
        cout << "Error, Input cannot be readd...\n";
        return -1;
    }

    //定义显示窗口
    namedWindow(argv[1], WINDOW_AUTOSIZE);
    namedWindow(argv[2], WINDOW_AUTOSIZE);

    imshow(argv[1],in_image);

    //创建Trackbar调整参数
    namedWindow("Trackbars");
    createTrackbar("L - H","Trackbars",&trackbas_l_h_min,179,onChangeTrackBar,&in_image);
    createTrackbar("L - S","Trackbars",&trackbas_l_s_min,255,onChangeTrackBar,&in_image);
    createTrackbar("L - V","Trackbars",&trackbas_l_v_min,255,onChangeTrackBar,&in_image);
    createTrackbar("U - H","Trackbars",&trackbas_u_h_min,179,onChangeTrackBar,&in_image);
    createTrackbar("U - S","Trackbars",&trackbas_u_s_min,179,onChangeTrackBar,&in_image);
    createTrackbar("U - V","Trackbars",&trackbas_u_v_min,179,onChangeTrackBar,&in_image);

    l_h = getTrackbarPos("L - H","Trackbars");
    l_s = getTrackbarPos("L - S","Trackbars");
    l_v = getTrackbarPos("L - V","Trackbars");
    u_h = getTrackbarPos("U - H","Trackbars");
    u_s = getTrackbarPos("U - S","Trackbars");
    u_v = getTrackbarPos("U - V","Trackbars");
    //定义灰度和显示灰度后图像
    cvtColor(in_image,hsv,COLOR_BGR2HSV);

    inRange(hsv,Scalar(l_h,l_s,l_v),Scalar(u_h,u_s,u_v),out_image);

    imshow("Trackbars", out_image);


    cout << "Press any key to exit...\n";

    waitKey(0);

    imwrite(argv[2],out_image);
    return 0;
}
