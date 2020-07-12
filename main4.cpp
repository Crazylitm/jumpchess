//
// Created by crazymike on 2019/10/5.
//
//
// Created by crazymike on 2019/10/3.
//
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <memory>
using namespace std;

// OpenCV includes
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "utils/MultipleImageWindow.h"

using namespace cv;
int thresholds=100;
RNG rng(12345);
Mat andCircle;

shared_ptr<MultipleImageWindow> miw;
const char* keys =
        {
                "{help h usage ? | | print this message}"
                        "{@image || Image to process}"
                        "{@lightPattern || Image light pattern to apply to image input}"
                        "{lightMethod | 1 | Method to remove backgroun light, 0 differenec, 1 div, 2 no light removal' }"
                        "{segMethod | 1 | Method to segment: 1 connected Components, 2 connectec components with stats, 3 find Contours }"
        };



Mat removebackgroudimg(Mat& srcimg ,int &numComp){
    //定义灰度和显示灰度后图像
    Mat hsv ,gray,out_image,canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    //1.将RGB的颜色转HSV颜色
    cvtColor(srcimg,gray,COLOR_BGR2HSV);


    //2.使用中值模糊函数进行噪音点去除操作
    Mat img_noise;
    medianBlur(gray,img_noise,3);
    miw->addImage("remove-gray-1",gray);

    //3.使用高斯模糊函数进行噪音点去除操作
    GaussianBlur(gray,hsv,Size(15,15),3,3);//高斯滤波

    //4.定义HSV的H颜色唯独把桌面的颜色和跳棋棋盘区分开
    inRange(hsv,Scalar(40,30,100),Scalar(200,200,255),out_image);

    //5.对图像进行线性分割
    Canny(out_image, canny_output, thresholds, thresholds*2, 3);
    //miw->addImage("canny_output",canny_output);

    //6.进行膨胀和侵蚀操作
    Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
    morphologyEx(canny_output, canny_output, MORPH_DILATE, k, Point(-1, -1));


    //7. 对图像中的物体进行轮廓线查找，生成到轮廓线数组中
    vector<Vec4i> hireachy;
    findContours(canny_output, contours, hireachy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
    if (contours.empty()) {
        return Mat();
    }

    //8.把轮廓线画出来。
    Mat markers(canny_output.size(), CV_32S);
    markers = Scalar::all(0);
    for (int i = 0; i < contours.size(); i++) {
        drawContours(markers, contours, i, Scalar(i + 1), -1, 8, hireachy, INT_MAX);
    }

    cout << "count2=" << contours.size() << endl;
    numComp = contours.size();

    // 分水岭变换
    watershed(srcimg, markers);

    //9.对物体进行颜色描述
    int numSegments = numComp;
    vector<Vec3b> colors;
    for (size_t i = 0; i < numSegments; i++) {
        int r = theRNG().uniform(0, 255);
        int g = theRNG().uniform(0, 255);
        int b = theRNG().uniform(0, 255);
        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }

    //10.颜色填充与最终显示
    Mat dst = Mat::zeros(markers.size(), CV_8UC3);
    int index = 0;
    for (int row = 0; row < markers.rows; row++) {
        for (int col = 0; col < markers.cols; col++) {
            index = markers.at<int>(row, col);
            if (index > 0 && index <= numSegments) {
                dst.at<Vec3b>(row, col) = colors[index - 1];
            }
            else {
                dst.at<Vec3b>(row, col) = Vec3b(255, 255, 255);
            }
        }
    }
    //miw->addImage("Output", dst);
    //imshow("Output",dst);
    Mat outreturn;
    {

        Mat gray,dst2,binary,dist;
        cvtColor(dst, gray, COLOR_BGR2GRAY);
        //miw->addImage("remove-gray-2",gray);

        gray = ~gray;

        Mat img_noise;
        medianBlur(gray,img_noise,3);
        //miw->addImage("remove-median-3",gray);

        GaussianBlur(img_noise,gray,Size(15,15),3,3);//高斯滤波
        miw->addImage("remove-gaussian-4",gray);

        threshold(gray, binary, 136, 255, THRESH_BINARY );
        miw->addImage("remove-threshold-5",binary);

        Canny(binary, dist, thresholds, thresholds*2, 3);
        //miw->addImage("dist2-4",dist);
        // 标记开始
        vector<vector<Point>> contours;
        vector<Vec4i> hireachy;
        findContours(dist, contours, hireachy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
        if (contours.empty()) {
            return Mat();
        }

        Mat markers(dist.size(), CV_32S);
        markers = Scalar::all(0);
        for (int i = 0; i < contours.size(); i++) {
            drawContours(markers, contours, i, Scalar(i + 1), -1, 8, hireachy, INT_MAX);
        }
        cout << "count3=" << contours.size() <<endl;

        //==========================
        numComp = contours.size();

        // 分水岭变换
        //watershed(srcimg, markers);

        //9.对物体进行颜色描述
        int numSegments = numComp;
        vector<Vec3b> colors;
        for (size_t i = 0; i < numSegments; i++) {
            int r = theRNG().uniform(0, 255);
            int g = theRNG().uniform(0, 255);
            int b = theRNG().uniform(0, 255);
            colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
        }

        //10.颜色填充与最终显示
        Mat dst = Mat::zeros(markers.size(), CV_8UC3);
        int index = 0;
        for (int row = 0; row < markers.rows; row++) {
            for (int col = 0; col < markers.cols; col++) {
                index = markers.at<int>(row, col);
                if (index > 0 && index <= numSegments) {
                    dst.at<Vec3b>(row, col) = colors[index - 1];
                }
                else {
                    dst.at<Vec3b>(row, col) = Vec3b(255, 255, 255);
                }
            }
        }
        //miw->addImage("last-6-Output", dst);


        //找到最大半径的圆
        Mat bigestbox,tempbox;
        RotatedRect bigestRect,temprect;
        int curi=0;

        for(int i =0 ;i < contours.size();i++){
            Mat(contours[i]).convertTo(tempbox, CV_32F);
            temprect = fitEllipse(tempbox);

            if(i==0){
                bigestbox = tempbox;
                bigestRect = temprect;
            } else{
                if(bigestRect.size.height  < temprect.size.height ||
                        bigestRect.size.width  < temprect.size.width){
                    bigestbox = tempbox;
                    bigestRect = temprect;
                    curi = i;
                }
            }
        }
        cout << "curi=" <<curi <<endl;
        //获得最大的半径的圆后进行抠图截取操作
        Mat pointsf;
        Mat(contours[curi]).convertTo(pointsf, CV_32F);
        RotatedRect box = fitEllipse(pointsf);
        cout << box.center;
        ellipse(dst,box,Scalar(0,0,255),5);
        //imshow("remove-koutu-6",dst);

        Mat circle1 =Mat::zeros(srcimg.rows,srcimg.cols, CV_8UC3);

        ellipse(circle1,box,Scalar(255,255,255),-1);
        //imshow("remove-ellipse-7",circle1);

        andCircle = circle1;
        bitwise_and(srcimg,circle1,outreturn);
        //imshow("remove-bitwise-8",outreturn);



    }



    /*
    Mat bgModel, fgModel;
    Mat result = Mat::zeros(srcimg.size(), CV_8UC1);
    grabCut(srcimg,result, Rect_<int>(0,0,3000,3000),bgModel, fgModel, 1, GC_INIT_WITH_RECT);
    //比较result的值为可能的前景像素才输出到result中
    compare(result, GC_PR_FGD, result, CMP_EQ);
    // 产生输出图像
    Mat foreground(srcimg.size(), CV_8UC3, Scalar(255, 255, 255));
    //将原图像src中的result区域拷贝到foreground中
    srcimg.copyTo(foreground, result);

    imshow("result2",result);
     */
    return outreturn;

}

Mat GetObjects(Mat& srcimg ,int &numComp){
    //定义灰度和显示灰度后图像
    Mat hsv ,gray,out_image,canny_output,dist;
    vector<vector<Point> > contours;
    vector<Vec4i> hireachy;

    //1.将RGB的颜色转HSV颜色
    cvtColor(srcimg,gray,COLOR_BGR2HSV);
    gray = ~gray;

    //2.使用中值模糊函数进行噪音点去除操作
    Mat img_noise;
    medianBlur(gray,img_noise,3);
    miw->addImage("get-gray-1",gray);



    GaussianBlur(gray,gray,Size(5,5),3,3);//高斯滤波
    miw->addImage("get-gaussian-2",gray);

    threshold(gray, hsv, 180, 255, THRESH_BINARY );
    miw->addImage("get-threshold-3",hsv);

    Canny(gray, dist, thresholds, thresholds*2, 3);
    miw->addImage("get-canny-4",dist);

    // 标记开始
    findContours(dist, contours, hireachy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
    if (contours.empty()) {
        return Mat();
    }

    Mat markers(dist.size(), CV_32S);
    markers = Scalar::all(0);
    for (int i = 0; i < contours.size(); i++) {
        drawContours(markers, contours, i, Scalar(i + 1), -1, 8, hireachy, INT_MAX);
    }
    circle(markers, Point(5, 5), 3, Scalar(255), -1);

    // 分水岭变换
    numComp = contours.size();


    // 分水岭变换
    watershed(srcimg, markers);

    //9.对物体进行颜色描述
    int numSegments = numComp;
    vector<Vec3b> colors;
    for (size_t i = 0; i < numSegments; i++) {
        int r = theRNG().uniform(0, 255);
        int g = theRNG().uniform(0, 255);
        int b = theRNG().uniform(0, 255);
        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }

    //10.颜色填充与最终显示
    Mat dst = Mat::zeros(markers.size(), CV_8UC3);
    int index = 0;
    for (int row = 0; row < markers.rows; row++) {
        for (int col = 0; col < markers.cols; col++) {
            index = markers.at<int>(row, col);
            if (index > 0 && index <= numSegments) {
                dst.at<Vec3b>(row, col) = colors[index - 1];
            }
            else {
                dst.at<Vec3b>(row, col) = Vec3b(255, 255, 255);
            }
        }
    }
    bitwise_and(dst,andCircle,dst);
    miw->addImage("get-bitwise-5", dst);
    imshow("get-bitwise-6",dst);
    cout << "count1=" << contours.size() << endl;

    return dst;

}
int main_2( int argc, const char** argv ) {

    int numSegments;

    CommandLineParser parser(argc, argv, keys);
    parser.about("Chapter 5-test. PhotoTool v1.0.1 for litm");
    //If requires help show
    if (parser.has("help")) {
        parser.printMessage();
        return 0;
    }

    String img_file = parser.get<String>(0);
    String light_pattern_file = parser.get<String>(1);
    auto method_light = parser.get<int>("lightMethod");
    auto method_seg = parser.get<int>("segMethod");

    // Check if params are correctly parsed in his variables
    if (!parser.check()) {
        parser.printErrors();
        return 0;
    }

    // Load image to process
    Mat img = imread(img_file);
    if (img.data == NULL) {
        cout << "Error loading image " << img_file << endl;
        return 0;
    }
    Mat img_grave = imread(img_file, 0);
    // Create the Multiple Image Window
    miw = make_shared<MultipleImageWindow>("Main window", 3, 4, WINDOW_AUTOSIZE);

    miw->addImage("Input", img);

    //GetObjects(img, numSegments);
    Mat obj1 = removebackgroudimg(img, numSegments);
    miw->addImage("Input-2",obj1);
    if(!obj1.empty() )
        GetObjects(obj1,numSegments);
    //finished
    miw->render();
    waitKey();
    return 0;
}