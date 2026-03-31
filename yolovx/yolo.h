#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>

// YOLO11n / YOLOv8 inference header
// Output format: [1, 4+nc, 8400]  (anchor-free, no obj_conf)
//
// className_jump_chess = { "B", "G", "O", "R", "R2", "W"};
// className_no_chess   = { "N" };
// enum ChessColor{ RED=1,BLUE=2,WHITE=3,GREEN=4,ORANGE=5,ROSERED=6,SHARELIGHTGREEN=0,NOCHESS=-1 }
enum YoloChessColor{ Y_BLUE=0, Y_GREEN=1, Y_ORANGE=2, Y_RED=3, Y_ROSERED=4, Y_WHITE=5, Y_NOCHESS=-1 };

struct Output {
    int id;
    float confidence;
    cv::Rect box;
    Output(int iid, float iconf, cv::Rect ibox){
        id = iid; confidence = iconf; box = ibox;
    }
    Output(){ id = -1; confidence = 0.0f; }
};

class Yolo {
public:
    Yolo() {}
    ~Yolo() {}
    bool readModel(cv::dnn::Net& net, std::string& netPath, bool isCuda);
    bool Detect(cv::Mat& SrcImg, cv::dnn::Net& net, std::vector<Output>& output, int iType=1);
    void drawPred(cv::Mat& img, std::vector<Output> result, std::vector<cv::Scalar> color, int iType=1);

private:
    // YOLO11n 输入尺寸
    const int netWidth  = 640;
    const int netHeight = 640;

    // 置信度阈值（与原 v5 保持相同）
    float classThreshold    = 0.25f;
    float nmsThreshold      = 0.45f;
    float nmsScoreThreshold = 0.25f * 0.25f;

    std::vector<std::string> className_jump_chess = { "B", "G", "O", "R", "R2", "W" };
    std::vector<std::string> className_no_chess   = { "N" };
};
