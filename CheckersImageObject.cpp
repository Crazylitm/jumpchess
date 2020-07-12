//
// Created by litm on 2020/2/8.
//

#include "CheckersImageObject.h"

void CheckersImageObject::readImage() {
    orgimage = imread("../inputImage/IMG_20190928_215912.jpg");
    Mat img_grave = imread("../inputImage/IMG_20190928_215912.jpg", 0);
    Mat org_bluechess  = imread("../inputImage/test2blue.png", -1);
    Mat dest_1;
    Mat dest_2;
    Mat dest_3;

    Mat src = orgimage;
    cvtColor(src,src,COLOR_BGR2GRAY);
    Mat template1 = org_bluechess;
    //cvtColor(template1,template1,COLOR_BGR2GRAY);
    int dstc= src.channels();
    int scrc= template1.channels();
    if ( dstc!= 3 ||  scrc!= 4)
    {
        printf("debug");
    }

    printf("\n");printf("\n");
    for(int x1=0;x1 < template1.rows;x1++){
        for(int x2=0;x2< template1.cols;x2++){
            printf("[");
            for(int x3=0;x3<4;x3++){
                printf("%d,",template1.at<Vec3b>(x1,x2)[x3]);
                template1.at<Vec3b>(x1,x2)[x3]=255;
            }
            printf("]");
        }
        printf("\n");
    }
    imshow("temp",template1);
    printf("\n");printf("\n");
    resize(img_grave,dest_2,Size(1200,800));

    Mat midImage,dstImage;
    bilateralFilter(orgimage,midImage,1,25,1);
    cvtColor(midImage,midImage,COLOR_BGR2GRAY);
    medianBlur(midImage,midImage,9);
    /*
    int ivalue =100;
    int ibright=50;
    for(int y =0 ; y<midImage.rows;y++){
        for(int x =0; x <midImage.cols;x++){
            for(int c=0; c<3;c++){
                midImage.at<Vec3b>(y,x)[c] = saturate_cast<uchar>((ivalue*0.01)*(midImage.at<Vec3b>(y,x)[c])+ibright);
            }
        }
    }*/

    GaussianBlur(midImage,midImage,Size(201,201),1,1);
    //Canny(midImage,midImage,10,20,3);
    //threshold(midImage, midImage, 136, 255, THRESH_BINARY );
    vector<Vec3f> circles;
    HoughCircles(midImage,circles,HOUGH_GRADIENT,1.5,1,30,100,10,100);

    int count = circles.size();
    for(size_t i =0 ; i < count; i++){
        Point center(cvRound(circles[i][0]),cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        if(radius >50) continue;
        circle(orgimage,center,3,Scalar(0,255,0),-1,8,0);
        circle(orgimage,center,radius,Scalar(155,50,255),3,8,0);
    }

    int result_cols = src.cols-template1.cols+1;
    int result_rows = src.rows-template1.rows+1;
    Mat result_1 = Mat(result_cols,result_cols,CV_32FC1);
    //matchTemplate(src,template1,result_1,TM_SQDIFF);
    normalize(result_1,result_1,0,255,NORM_MINMAX,-1,Mat());
    Point matchLoc(result_1.cols,result_1.rows);

    rectangle(src,matchLoc,Point(matchLoc.x+template1.cols,matchLoc.y+template1.rows),Scalar(0,0,0));


    resize(midImage,dest_3,Size(1200,800));
    resize(orgimage,dest_1,Size(1200,800));
    resize(src,src,Size(1200,800));

    imshow("read",dest_1);
    imshow("dest2",dest_2);
    imshow("dest3",dest_3);
    imshow("dest4",src);
}
void CheckersImageObject::test2() {
    RNG g_rng(12345);
    Mat g_srcImage = imread("../inputImage/IMG_20190928_215912.jpg",1);
    Mat g_grayImage;
    cvtColor(g_srcImage,g_grayImage,COLOR_BGR2GRAY);
    blur(g_grayImage,g_grayImage,Size(3,3));



    Mat threshold_output;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    threshold(g_grayImage,threshold_output,40,255,THRESH_BINARY);

    findContours(threshold_output,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE,Point(0,0));

    vector<vector<Point>> contours_poly(contours.size());
    vector<Rect> boundRect(contours.size());
    vector<Point2f>center(contours.size());
    vector<float>radius(contours.size());

    for(unsigned int i=0; i<contours.size();i++){
        approxPolyDP(Mat(contours[i]),contours_poly[i],3,true);
        boundRect[i] = boundingRect(Mat(contours_poly[i]));
        minEnclosingCircle(contours_poly[i],center[i],radius[i]);

    }

    Mat drawing = Mat::zeros(threshold_output.size(),CV_8UC3);
    for(int unsigned i=0;i<contours.size();i++){
        if((int)radius[i] >100 || (int)radius[i]<34)continue;
        Scalar color = Scalar(0,255,255);
        //drawContours(drawing,contours_poly,i,color,1,8,vector<Vec4i>(),0,Point());
        //rectangle(drawing,boundRect[i].tl(),boundRect[i].br(),color,2,8,0);
        circle(g_srcImage,center[i],(int)radius[i],color,4,8,0);
    }
    resize(threshold_output,threshold_output,Size(1200,800));
    resize(g_srcImage,g_srcImage,Size(1200,800));
    imshow("test2",threshold_output);
    imshow("test1",g_srcImage);
}
void CheckersImageObject::calcHistNow() {
    Mat srcImage;
    srcImage = imread("../inputImage/IMG_20190928_215912.jpg");

    int bins =256;
    int hist_size[] = {bins};
    float range[] = {0,256};
    const float* ranges[] = {range};
    MatND redHist,grayHist,blueHist;
    int channels_r[] ={0};

    calcHist(&srcImage,1,channels_r,Mat(),
            redHist,1,hist_size,ranges,
            true,false);

    int channels_g[1] = {1};
    calcHist(&srcImage,1,channels_g,Mat(),
            grayHist,1,hist_size,ranges,
            true,
            false);

    int channels_b[] = {2};
    calcHist(&srcImage,1,channels_b,Mat(),
            blueHist,1,hist_size,ranges,
            true,
            false);

    double maxValue_red,maxValue_green,maxValue_blue;
    minMaxLoc(redHist,0,&maxValue_red,0,0);
    minMaxLoc(grayHist,0,&maxValue_green,0,0);
    minMaxLoc(blueHist,0,&maxValue_blue,0,0);

    int scale =1;
    int histHeight =256;
    Mat histImage = Mat::zeros(histHeight,bins*3,CV_8UC3);

    for(int i=0;i<bins;i++){
        float binValue_red = redHist.at<float>(i);
        float binValue_green = grayHist.at<float>(i);
        float binValue_blue = blueHist.at<float>(i);
        int intensity_red = cvRound(binValue_red*histHeight/maxValue_red);
        int intensity_green = cvRound(binValue_green*histHeight/maxValue_green);
        int intensity_blue = cvRound(binValue_blue*histHeight/maxValue_blue);

        rectangle(histImage,Point(i*scale,histHeight-1),
                Point((i+1)*scale -1, histHeight - intensity_red),
                Scalar(255,0,0));

        rectangle(histImage,Point((i+bins)*scale,histHeight-1),
                  Point((i+bins+1)*scale -1, histHeight - intensity_green),
                  Scalar(0,255,0));

        rectangle(histImage,Point((i+bins*2)*scale,histHeight-1),
                  Point((i+bins*2+1)*scale -1, histHeight - intensity_blue),
                  Scalar(0,0,255));
    }



    Mat srcImage2;
    srcImage2 = imread("../inputImage/IMG_20190928_215912.jpg",0);


    Mat cornerStrength;
    cornerHarris(srcImage2,cornerStrength,2,3,0.04);

    Mat harrisCorner;
    threshold(cornerStrength,harrisCorner,0.00001,255,THRESH_BINARY);

    Mat dstImage;
    Mat src2Image;
    cvtColor(srcImage,src2Image,COLOR_BGR2GRAY);
    equalizeHist(src2Image,dstImage);


    resize(srcImage,srcImage,Size(1200,800));
    resize(dstImage,dstImage,Size(1200,800));
    resize(cornerStrength,cornerStrength,Size(1200,800));
    resize(harrisCorner,harrisCorner,Size(1200,800));
    //resize(g_srcImage,g_srcImage,Size(1200,800));
    imshow("Org",srcImage);
    imshow("直方图",histImage);
    imshow("Dst",dstImage);
    imshow("corner",cornerStrength);
    imshow("harris",harrisCorner);


}
void onChangeTrackBar(int pos, void *usrdata) {
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


    resize(out,out,Size(1000,600));
    imshow("Trackbars", out);
}
void CheckersImageObject::findChessColor_v2(ChessColor color) {
    Mat in_image,hsv,out_image,out;
    int l_h,l_s,l_v,u_s,u_h,u_v;

    in_image = imread("../inputImage/IMG_20190928_215711.jpg",IMREAD_UNCHANGED);
    resize(in_image,in_image,Size(1000,600));
    imshow("argv[1]",in_image);

    switch(color){
        case RED:
            l_h =0;
            l_s =197;
            l_v =55;
            u_h =2;
            u_s =249;
            u_v =139;
            break;
        case BLUE:
            l_h =84;
            l_s =228;
            l_v =194;
            u_h =124;
            u_s =255;
            u_v =222;
            break;
        case ORANGE:
            l_h =3;
            l_s =152;
            l_v =121;
            u_h =14;
            u_s =255;
            u_v =255;
            break;
        case WHITE:
            l_h =102;
            l_s =46;
            l_v =220;
            u_h =115;
            u_s =49;
            u_v =234;

            break;

        case ROSERED:
            l_h =154;
            l_s =189;
            l_v =131;
            u_h =164;
            u_s =255;
            u_v =176;
            break;

        case GREEN:
            l_h =65;
            l_s =196;
            l_v =82;
            u_h =74;
            u_s =240;
            u_v =129;
            break;

        default:
            l_h =0;
            l_s =197;
            l_v =55;
            u_h =2;
            u_s =249;
            u_v =139;
            break;
    }

    //定义灰度和显示灰度后图像
    cvtColor(in_image,hsv,COLOR_BGR2HSV);

    //imshow("hsv", hsv);

    inRange(hsv,Scalar(l_h,l_s,l_v),Scalar(u_h,u_s,u_v),out_image);



    //bitwise_not(out_image,out_image);

    imshow("out_image", out_image);
    int count1=0;
    //Mat teral = out_image;
    for(int x1=0;x1 < out_image.rows;x1++){
        for(int x2=0;x2< out_image.cols;x2++){
            if(out_image.at<Vec3b>(x1,x2)[0]==0 &&
               out_image.at<Vec3b>(x1,x2)[1]==0 &&
               out_image.at<Vec3b>(x1,x2)[2]==0 &&
               out_image.at<Vec3b>(x1,x2)[3]==0)
                continue;
            printf("[");
            for(int x3=0;x3<4;x3++){
                printf("%d,",out_image.at<Vec3b>(x1,x2)[x3]);
                //out.at<Vec3b>(x1,x2)[x3]=255;
                count1++;
            }
            printf("]");
        }
        printf("count=%d\n",count1);
    }
    printf("count=%d\n",count1);
    bitwise_and(hsv,hsv,out,out_image);

    Mat blackimage = Mat::zeros(out_image.rows,out_image.cols,CV_8UC3);
    //resize(out,out,Size(1000,600));



    imshow("out", out);


    GaussianBlur(out_image,out_image,Size(3,3),0,0);
    Canny(out_image,out_image,3,9,3);
    imshow("out_image2q",out_image);


    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    Mat distImage = Mat::zeros(out_image.rows,out_image.cols,CV_8UC3);
    findContours(out_image,contours,hierarchy,RETR_LIST,CHAIN_APPROX_SIMPLE,Point(0,0));
    int index=0;
    for(;index>0;index=hierarchy[index][0]){
        Scalar color(rand()&255,rand()&255,rand()&255);
        drawContours(distImage,contours,index,color,FILLED,8,hierarchy);

    }

    imshow("distImage",distImage);


    vector<Vec3f> circles;
    imshow("out_image_223",out_image);
    HoughCircles(out_image,circles,HOUGH_GRADIENT,1,10,10,20,5,50);
    size_t count = circles.size();
    for(size_t i =0;i<circles.size();i++){
        Point center(cvRound(circles[i][0]),cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        if(radius >13) continue;
        circle(out_image,center,3,Scalar(100,255,0),-1,8,0);
        circle(out_image,center,radius,Scalar(155,135,255),3,3,0);

    }
    imshow("out_image_Hough",out_image);
/*
    Mat cannyImage= out_image;
    Mat edge,dist;
    blur(cannyImage,edge,Size(3,3));

    Canny(edge,edge,3,9,3);
    dist = Scalar::all(0);

    imshow("edge",edge);
*/

}
void CheckersImageObject::findChessColor(ChessColor color) {
    Mat in_image,hsv, out_image;

    int trackbas_l_h_min = 0;
    int trackbas_l_s_min = 0;
    int trackbas_l_v_min = 0;
    int trackbas_u_s_min = 255;
    int trackbas_u_h_min = 255;
    int trackbas_u_v_min = 255;

    int l_h,l_s,l_v,u_s,u_h,u_v;

    //读取图片
    //in_image = imread("../inputImage/IMG_20190928_215912.jpg", IMREAD_UNCHANGED);
    in_image = imread("../inputImage/IMG_20190928_215711.jpg",IMREAD_UNCHANGED);

    resize(in_image,in_image,Size(1000,600));
    imshow("argv[1]",in_image);

    //创建Trackbar调整参数
    namedWindow("Trackbars");
    createTrackbar("L - H","Trackbars",&trackbas_l_h_min,255,onChangeTrackBar,&in_image);
    createTrackbar("L - S","Trackbars",&trackbas_l_s_min,255,onChangeTrackBar,&in_image);
    createTrackbar("L - V","Trackbars",&trackbas_l_v_min,255,onChangeTrackBar,&in_image);
    createTrackbar("U - H","Trackbars",&trackbas_u_h_min,255,onChangeTrackBar,&in_image);
    createTrackbar("U - S","Trackbars",&trackbas_u_s_min,255,onChangeTrackBar,&in_image);
    createTrackbar("U - V","Trackbars",&trackbas_u_v_min,255,onChangeTrackBar,&in_image);

    l_h = getTrackbarPos("L - H","Trackbars");
    l_s = getTrackbarPos("L - S","Trackbars");
    l_v = getTrackbarPos("L - V","Trackbars");
    u_h = getTrackbarPos("U - H","Trackbars");
    u_s = getTrackbarPos("U - S","Trackbars");
    u_v = getTrackbarPos("U - V","Trackbars");
    //定义灰度和显示灰度后图像
    cvtColor(in_image,hsv,COLOR_BGR2HSV);

    inRange(hsv,Scalar(l_h,l_s,l_v),Scalar(u_h,u_s,u_v),out_image);




    resize(out_image,out_image,Size(1000,600));

    imshow("Trackbars", out_image);



    cout << "Press any key to exit...\n";
    waitKey();
}