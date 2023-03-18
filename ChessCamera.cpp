//
// Created by litm on 2020/7/16.
//

#include "ChessCamera.h"

void ChessCamera::init_vtest() {
    string gst = "nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)640, height=(int)480, format=(string)I420, framerate=(fraction)30/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)I420 ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";

        VideoCapture capture;

     //capture.set(CAP_PROP_FOURCC,VideoWriter::fourcc('M','J','P','G'));
        capture.open(2,CAP_V4L);//2
        Mat edges;
         if( !capture.isOpened() ) return;
         double rate = capture.get(CAP_PROP_FPS);
    //  capture.set(CAP_PROP_FPS,30);
     capture.set(CAP_PROP_FRAME_HEIGHT,768);//1080
     capture.set(CAP_PROP_FRAME_WIDTH,1024);//1920
        Mat frame;
        int delay = 1000/rate;
  while(1){
    capture >> frame;
    if(frame.empty())break;
      // namedWindow("Camera",WINDOW_NORMAL);

            cvtColor(frame,edges,COLOR_BGR2GRAY);
            blur(edges,edges,Size(7,7));
            Canny(edges,edges,0,30,3);

            imshow("Camera",edges);
            waitKey(delay);
  }





        return;

}
void ChessCamera::init(map<int,list<CircleReturn>>* map,FrameProcessor& segmentor ) {
    displayInput(WINDOW_NAME_CHESS_INPUT);
    displayOutput(WINDOW_NAME_CHESS_OUTPUT);
    //导入相机内参和畸变系数矩阵
    /*
    FileStorage file_storage("/home/litm/CLionProjects/MyBiaoDing/Intrinsic.xml", FileStorage::READ);
    file_storage["CameraMatrix"] >> camera_matrix;
    file_storage["Dist"] >> distortion_coefficients;
    file_storage.release();
    */

    if(setInput(2)){
        double rate = capture.get(CAP_PROP_FPS);
        setDelay(1000/rate);
        stopAtFrameNo(-1);
        //BGFGSegmentor segmentor;
        segmentor.setThreshold(25);
        //initMouseParam();
        capture.set(CAP_PROP_FRAME_HEIGHT,768);//1080
        capture.set(CAP_PROP_FRAME_WIDTH,1024);//1920
        setFrameProcessor(&segmentor);
        setMemMap(map);//BGFGSegmentor.setMemMap() ;
        segmentor.ReadIniToSaveInfo();
        run();
    }


}
void ChessCamera::DoUndistort(Mat& src, Mat& out) {
    cv::undistort(src, out, camera_matrix, distortion_coefficients);
}
void ChessCamera::run() {
    Mat frame;
    Mat output;
    Mat unframe;

    if(!isOpened()){
        return;
    }
    stop = false;

    while(!isStopped()){
        if(!readNextFrame(frame))
            break;
        flip(frame,frame,-1);//上下左右镜像
        if(windowNameInput.length() !=0)
        {
            if(frame.empty() !=true){
                // unframe = frame.clone();
                // DoUndistort(frame,unframe);
                // frame = unframe;
                //imshow(windowNameInput,frame);
            }

        }

        if(callIt){

            if(process)
                process(frame,output);
            else if(frameProcessor)
                frameProcessor->process(frame,output);
            fnumber++;
        } else{
            output = frame;
        }

        if(windowNameOutput.length() !=0){
            if (output.empty() != true) {
                //imshow(windowNameOutput, old_last_frame);
                imshow(WINDOW_NAME_CHESS_OUTPUT, output);
            }
            /*
            if(windowsNameDealIn.length() !=0)
                imshow(windowsNameDealIn,frame);
                */


            if(delay >0 && waitKey(delay) >=0){
                //stopIt();
            }

            if(frameToStop >=0 && getFrameNumber() == fnumber)
                stopIt();
        }
        //waitKey(30);
    }
}
