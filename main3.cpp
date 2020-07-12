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



shared_ptr<MultipleImageWindow> miw;
int thresholds=100;
RNG rng(12345);

const char* keys =
        {
                "{help h usage ? | | print this message}"
                        "{@image || Image to process}"
                        "{@lightPattern || Image light pattern to apply to image input}"
                        "{lightMethod | 1 | Method to remove backgroun light, 0 differenec, 1 div, 2 no light removal' }"
                        "{segMethod | 1 | Method to segment: 1 connected Components, 2 connectec components with stats, 3 find Contours }"
        };


static Scalar randomColor( RNG& rng )
{
    auto icolor = (unsigned) rng;
    return Scalar( icolor&255, (icolor>>8)&255, (icolor>>16)&255 );
}

void ConnecteMyVersion1(Mat img){

    Mat bright,brightHSV,brightYCB,brightLAB;

    cvtColor(img,bright,COLOR_RGB2BGR);
    cvtColor(img,brightHSV,COLOR_RGB2HSV);
    cvtColor(img,brightYCB,COLOR_RGB2YCrCb);
    cvtColor(img,brightLAB,COLOR_RGB2Lab);

//C++ code
    //[1185:1398]--H:S:V--[ H:65,S:159,V:138]--R:G:B--[ R:67,G:138,B:52]
    cv::Vec3b bgrPixel(67, 138, 52);
    Vec3b inithsvPixel(65,159,138);

// Create Mat object from vector since cvtColor accepts a Mat object

    Mat3b bgr (bgrPixel);

    Mat3b inithsv (inithsvPixel);

//Convert pixel values to other color spaces.

    Mat3b hsv,ycb,lab;

    cvtColor(inithsv,bgr,COLOR_HSV2BGR);

    cvtColor(bgr, ycb, COLOR_BGR2YCrCb);

    cvtColor(bgr, hsv, COLOR_BGR2HSV);

    hsv = inithsv;

    cvtColor(bgr, lab, COLOR_BGR2Lab);



    //Get back the vector from Mat

    Vec3b hsvPixel(hsv.at<Vec3b>(0,0));

    Vec3b ycbPixel(ycb.at<Vec3b>(0,0));

    Vec3b labPixel(lab.at<Vec3b>(0,0));

    int thresh = 20;


    //------------BGR------------------
    cv::Scalar minBGR = cv::Scalar(bgrPixel.val[0] - thresh, bgrPixel.val[1] - thresh, bgrPixel.val[2] - thresh);

    cv::Scalar maxBGR = cv::Scalar(bgrPixel.val[0] + thresh, bgrPixel.val[1] + thresh, bgrPixel.val[2] + thresh);


    cv::Mat maskBGR, resultBGR;

    cv::inRange(bright, minBGR, maxBGR, maskBGR);

    cv::bitwise_and(bright, bright, resultBGR, maskBGR);

    //------------HSV----------------
    cv::Scalar minHSV = cv::Scalar(hsvPixel.val[0] - thresh, hsvPixel.val[1] - thresh, hsvPixel.val[2] - thresh);

    cv::Scalar maxHSV = cv::Scalar(hsvPixel.val[0] + thresh, hsvPixel.val[1] + thresh, hsvPixel.val[2] + thresh);

    cv::Mat maskHSV, resultHSV;

    cv::inRange(brightHSV, minHSV, maxHSV, maskHSV);

    cv::bitwise_and(brightHSV, brightHSV, resultHSV, maskHSV);


    //---------YCB----------
    cv::Scalar minYCB = cv::Scalar(ycbPixel.val[0] - thresh, ycbPixel.val[1] - thresh, ycbPixel.val[2] - thresh);

    cv::Scalar maxYCB = cv::Scalar(ycbPixel.val[0] + thresh, ycbPixel.val[1] + thresh, ycbPixel.val[2] + thresh);

    cv::Mat maskYCB, resultYCB;

    cv::inRange(brightYCB, minYCB, maxYCB, maskYCB);

    cv::bitwise_and(brightYCB, brightYCB, resultYCB, maskYCB);


    //------LAB-------------
    cv::Scalar minLAB = cv::Scalar(labPixel.val[0] - thresh, labPixel.val[1] - thresh, labPixel.val[2] - thresh);

    cv::Scalar maxLAB = cv::Scalar(labPixel.val[0] + thresh, labPixel.val[1] + thresh, labPixel.val[2] + thresh);

    cv::Mat maskLAB, resultLAB;

    cv::inRange(brightLAB, minLAB, maxLAB, maskLAB);

    cv::bitwise_and(brightLAB, brightLAB, resultLAB, maskLAB);

    //------------Show-----------
    miw->addImage("Result BGR", resultBGR);

    miw->addImage("Result HSV", resultHSV);

    miw->addImage("Result YCB", resultYCB);

    miw->addImage("Output LAB", resultLAB);
}
void ConnectedComponents(Mat img){
    Mat labels;
    auto num_objects = connectedComponents(img,labels);
    if(num_objects <2){
        cout <<"No objects detected" << endl;
        return;
    } else{
        cout << "Number of objects detected:" << num_objects -1 <<endl;
    }

    Mat output = Mat::zeros(img.rows,img.cols,CV_8UC3);
    RNG rng(0xFFFFFFFF);
    for(auto i = 1; i < num_objects; i++){
        Mat mask = labels ==i;
        output.setTo(randomColor(rng),mask);
    }
    miw->addImage("Result",output);
}
void onChangeTrackbar(int pos, void* userdata){

}
Mat watershedCluster(Mat &image, int &numComp) {
    // 二值化
    Mat gray, binary;
    Mat dist;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    miw->addImage("gray1",gray);

    gray = ~gray;

    GaussianBlur(gray,gray,Size(5,5),3,3);//高斯滤波
    miw->addImage("gray2",gray);

    threshold(gray, binary, 180, 255, THRESH_BINARY );
    miw->addImage("binary3",binary);
    /*
    // 形态学与距离变换
    Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
    morphologyEx(binary, binary, MORPH_DILATE, k, Point(-1, -1));

    distanceTransform(binary, dist, DistanceTypes::DIST_L2, 3, CV_32F);
    //miw->addImage("dist4",dist);
    normalize(dist, dist, 0.0, 1.0, NORM_MINMAX);

    // 开始生成标记
    threshold(dist, dist, 0.1, 1.0, THRESH_BINARY);
    normalize(dist, dist, 0, 255, NORM_MINMAX);
    dist.convertTo(dist, CV_8UC1);
    miw->addImage("dist5",dist);
    */
    Canny(gray, dist, thresholds, thresholds*2, 3);
    miw->addImage("dist6",dist);
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
    circle(markers, Point(5, 5), 3, Scalar(255), -1);

    // 分水岭变换
    watershed(image, markers);
    numComp = contours.size();
    return markers;
}

void createDisplaySegments(Mat &markers, int numSegments, Mat &image) {
    // generate random color
    vector<Vec3b> colors;
    for (size_t i = 0; i < numSegments; i++) {
        int r = theRNG().uniform(0, 255);
        int g = theRNG().uniform(0, 255);
        int b = theRNG().uniform(0, 255);
        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }

    // 颜色填充与最终显示
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

    //-------
    {

        Mat gray,dst2,binary,dist;
        cvtColor(dst, gray, COLOR_BGR2GRAY);
        miw->addImage("gray2-1",gray);

        gray = ~gray;

        Mat img_noise;
        medianBlur(gray,img_noise,3);
        miw->addImage("gray2-1-2",gray);

        GaussianBlur(img_noise,gray,Size(15,15),3,3);//高斯滤波
        miw->addImage("gray2-2",gray);

        threshold(gray, binary, 136, 255, THRESH_BINARY );
        miw->addImage("binary2-3",binary);

        Canny(binary, dist, thresholds, thresholds*2, 3);
        //miw->addImage("dist2-4",dist);
        // 标记开始
        vector<vector<Point>> contours;
        vector<Vec4i> hireachy;
        findContours(dist, contours, hireachy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
        if (contours.empty()) {
            return ;
        }

        Mat markers(dist.size(), CV_32S);
        markers = Scalar::all(0);
        for (int i = 0; i < contours.size(); i++) {
            drawContours(markers, contours, i, Scalar(i + 1), -1, 8, hireachy, INT_MAX);
        }
        cout << "count=" << contours.size() <<endl;

    }

    imshow("分水岭图像分割-演示",dst);
    return;
}

// 回调函数
void thresh_callback(int pos,void* usrdata )
{
    Mat canny_output,gray;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    Mat src_gray =  *(Mat *) (usrdata);

    GaussianBlur(src_gray,gray,Size(15,15),3,3);//高斯滤波
    // canny 边缘检测
    Canny(gray, canny_output, thresholds, thresholds*2, 3);

    // 寻找轮廓
    findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3);

    // 画出轮廓
    for( size_t i = 0; i< contours.size(); i++ ) {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point() );
    }

    //namedWindow( "Contours", WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );
}

Mat removebackgroudimg(Mat& srcimg ,int &numComp){
    //定义灰度和显示灰度后图像
    Mat hsv ,out_image,canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    cvtColor(srcimg,hsv,COLOR_BGR2HSV);

    GaussianBlur(hsv,hsv,Size(15,15),3,3);//高斯滤波

    inRange(hsv,Scalar(40,30,100),Scalar(200,200,255),out_image);
    Canny(out_image, canny_output, thresholds, thresholds*2, 3);
    miw->addImage("canny_output",canny_output);

    Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
    morphologyEx(canny_output, canny_output, MORPH_DILATE, k, Point(-1, -1));

    // 寻找轮廓
    findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3);

    // 画出轮廓
    for( size_t i = 0; i< contours.size(); i++ ) {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point() );
    }

    //namedWindow( "Contours", WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );



    imshow("Trackbars", out_image);


    // 标记开始

    vector<Vec4i> hireachy;
    findContours(canny_output, contours, hireachy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
    if (contours.empty()) {
        return Mat();
    }

    Mat markers(canny_output.size(), CV_32S);
    markers = Scalar::all(0);
    for (int i = 0; i < contours.size(); i++) {
        drawContours(markers, contours, i, Scalar(i + 1), -1, 8, hireachy, INT_MAX);
    }
    //circle(markers, Point(5, 5), 3, Scalar(255), -1);

    // 分水岭变换
    //watershed(srcimg, markers);
    cout << contours.size();
    numComp = contours.size();
    return markers;

}
int main3( int argc, const char** argv )
{
    CommandLineParser parser(argc, argv, keys);
    parser.about("Chapter 5-test. PhotoTool v1.0.1 for litm");
    //If requires help show
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    String img_file= parser.get<String>(0);
    String light_pattern_file= parser.get<String>(1);
    auto method_light= parser.get<int>("lightMethod");
    auto method_seg= parser.get<int>("segMethod");

    // Check if params are correctly parsed in his variables
    if (!parser.check())
    {
        parser.printErrors();
        return 0;
    }

    // Load image to process
    Mat img= imread(img_file);
    if(img.data==NULL){
        cout << "Error loading image "<< img_file << endl;
        return 0;
    }
    Mat img_grave = imread(img_file,0);
    // Create the Multiple Image Window
    miw= make_shared<MultipleImageWindow>("Main window", 3, 2, WINDOW_AUTOSIZE);

    createTrackbar("Canny:","Main window",&thresholds,255,thresh_callback,&img);

    Mat img_noise;
    medianBlur(img_grave,img_noise,3);

    //show image
    miw->addImage("Input", img_noise);
    //miw->addImage("noise", img_noise);
    //ConnectedComponents(img_noise);
    //ConnecteMyVersion1(img);
    int numSegments;
    // Mat markers = watershedCluster(img, numSegments);
    Mat markers = removebackgroudimg(img,numSegments);
    createDisplaySegments(markers, numSegments, img);


    //finished
    miw->render();
    waitKey();
    return 0;

}