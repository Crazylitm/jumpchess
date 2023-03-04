#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>


//className_jump_chess = { "B", "G", "O", "R", "R2", "W"};
//className_no_chess = { "N"};
//enum ChessColor{ RED=1, BLUE=2, WHITE=3, GREEN=4,ORANGE=5,ROSERED=6,SHARELIGHTGREEN=0,NOCHESS=-1}; define Chessinclue.h;
enum YoloChessColor{ Y_BLUE=0,Y_GREEN=1,Y_ORANGE=2,Y_RED=3,Y_ROSERED=4,Y_WHITE=5,Y_NOCHESS=-1};

struct Output {
	int id;             //结果类别id
	float confidence;   //结果置信度
	cv::Rect box;       //矩形框
    Output(int iid,float iconf,cv::Rect ibox){
        id = iid;
        confidence = iconf;
        box = ibox;
    }
    Output(){
        id = -1;
        confidence = 0.0;
    }
};

class Yolo {
public:
	Yolo() {
	}
	~Yolo() {}
	bool readModel(cv::dnn::Net& net, std::string& netPath, bool isCuda);
	bool Detect(cv::Mat& SrcImg, cv::dnn::Net& net, std::vector<Output>& output,int iType=1);//if iType =1,detect Net of Chess; iType=1,detect Net of No chess
	void drawPred(cv::Mat& img, std::vector<Output> result, std::vector<cv::Scalar> color,int iType=1);//if iType =1,detect Net of Chess; iType=1,detect Net of No chess

private:
#if(defined YOLO_P6 && YOLO_P6==true)
	const float netAnchors[4][6] = { { 19,27, 44,40, 38,94 },{ 96,68, 86,152, 180,137 },{ 140,301, 303,264, 238,542 },{ 436,615, 739,380, 925,792 } };

	const int netWidth = 1280;  //ONNX图片输入宽度
	const int netHeight = 1280; //ONNX图片输入高度

	const int strideSize = 4;  //stride size
#else
	const float netAnchors[3][6] = { { 10,13, 16,30, 33,23 },{ 30,61, 62,45, 59,119 },{ 116,90, 156,198, 373,326 } };
	
	const int netWidth = 640;   //ONNX图片输入宽度
	const int netHeight = 640;  //ONNX图片输入高度
	
	const int strideSize = 3;   //stride size
#endif // YOLO_P6

	const float netStride[4] = { 8, 16.0,32,64 };

	float boxThreshold = 0.25;
	float classThreshold = 0.25;

	float nmsThreshold = 0.45;
	float nmsScoreThreshold = boxThreshold * classThreshold;

	std::vector<std::string> className_jump_chess = { "B", "G", "O", "R", "R2", "W"};
    std::vector<std::string> className_no_chess = { "N"};
};
