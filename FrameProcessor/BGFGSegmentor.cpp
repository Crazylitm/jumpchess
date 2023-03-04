//
// Created by litm on 2020/7/18.
//

#include "BGFGSegmentor.h"
bool BGFGSegmentor::judgeLastoneAndThisoneEqual(vector<Output>& lastone,vector<Output>& thisone) {
        if(lastone.size() != thisone.size())
            return false;
        int findEqualSum=0;
        for(int i =0 ; i < thisone.size();  i++){
            for(int j =0 ; j < lastone.size(); j++){

                 if(rectA_intersect_rectB(thisone[i].box,lastone[j].box)){
                     findEqualSum++;
                     continue;
                 }
                if(thisone[i].box.x == lastone[j].box.x &&
                   thisone[i].box.y == lastone[j].box.y){
                    findEqualSum++;

                }
            }
        }
        if(findEqualSum == thisone.size() )
            return true;

    return false;
}
void BGFGSegmentor::process(Mat &input, Mat &output) {
    //process_v1(input,output);
    //process_v2(input,output);
    //input = in_fun;
    //process_v3(in_fun,output);

    //LocatChessCircle(input,output);
    vector<Output> result;
    static vector<Output> lastoneResult;
    Mat input_save;
    vector<Output> result_nochess;
    vector<Output> result_nochess_v2;
    vector<Output> result_nochess_v3;
    output = input;//save input data to output,for do not update

    input_save = input.clone();
    if (test.Detect(input, net, result)) {
        test.drawPred(input, result, color);
        output = input;
    }
    else {
        cout << "Detect jump chess Failed!"<<endl;

    }


    if (test.Detect(input_save, net_no_ches, result_nochess,2)) {
        //
        vector<Output>::iterator itor;
        int i =0;
        for(itor=result_nochess.begin(); itor != result_nochess.end();itor++,i++ ){
            result_nochess_v2.push_back(result_nochess[i]);
            for(int j=0; j <result.size();j++){
                if(rectA_intersect_rectB(result_nochess[i].box,result[j].box)){
                    result_nochess_v2.pop_back();
                }
            }
        }

        i =0;
        int j =0;
        vector<Output>::iterator itor_j;
        for(itor=result_nochess_v2.begin(); itor != result_nochess_v2.end();itor++,i++ ){
            result_nochess_v3.push_back(result_nochess_v2[i]);
            for(j=i+1,itor_j=result_nochess_v2.begin(); itor_j !=result_nochess_v2.end();itor_j++,j++){

                if(rectA_intersect_rectB(result_nochess_v2[i].box,result_nochess_v2[j].box)){
                        result_nochess_v3.pop_back();
                }
            }
        }
        if(result_nochess_v2.size() != result_nochess_v3.size()){
            cout << "find duplicate chess" << endl;
        }
        test.drawPred(output, result_nochess_v3, color,2);
    }
    else {
        cout << "Detect no chess Failed!"<<endl;

    }

    //add 2023.2.5 for check chess ui of yolov5 detect
    outPutUi.setOutputMat(output);
    outPutUi.DrawBackground();
    outPutUi.DrawButton(ButtonTypeStatus);
    outPutUi.setDetectResult(result);
    outPutUi.setDetectResult_no_chess(result_nochess_v3);

    //if have read the parse config or parse the init,to flag =true;
    if(lastoneResult.size() <=0)
        lastoneResult.assign(result.begin(),result.end());
    if(had_save_flag && (judgeLastoneAndThisoneEqual(lastoneResult,result) == false || result.size()<=0)){
        outPutUi.setSaveFlag(had_save_flag);
        outPutUi.parseDetectChessPositionToBoardUpdateFromSaveResult(result,result_nochess_v3);
        update();
        lastoneResult.clear();
        lastoneResult.assign(result.begin(),result.end());
        //imshow(WINDOW_NAME_CHESS_OUTPUT,outPutUi.getOutputMat());//刷新以下
    }

}
void BGFGSegmentor::init(){
    initMouseParam();
}
void BGFGSegmentor::rotate(cv::Mat src, cv::Mat& dst, double angle)
{
    cv::Size src_sz = src.size();
    cv::Size dst_sz(src_sz.width, src_sz.height);

    cv::Point center = cv::Point(src.cols / 2, src.rows / 2);//旋转中心
    cv::Mat rot_mat = cv::getRotationMatrix2D(center, angle, 1.0);//获得仿射变换矩阵

    cv::warpAffine(src, dst, rot_mat, dst_sz);
}

void BGFGSegmentor::initMouseParam() {
    setMouseCallback(WINDOW_NAME_CHESS_OUTPUT,onMouseHandle_BGFGSegmentor_imShow,(void*)this);
    //setMouseCallback(WINDOW_NAME_CHESS_INPUT,onMouseHandle_Camera_imShow_in,(void*)this);
    printf("initMouseParam\n") ;

}
void BGFGSegmentor::onMouseHandle_BGFGSegmentor_inner(int event, int x, int y, int flags, void *param){
    switch(event){
        case EVENT_MOUSEMOVE:
        {

        }
            break;
        case EVENT_LBUTTONUP:
        {
            //printf("EVENT_LBUTTONUP\n") ;
            outPutUi.DrawButton(1);
            ButtonTypeStatus = 1;
            imshow(WINDOW_NAME_CHESS_OUTPUT,outPutUi.getOutputMat());//刷新以下
        }
            break;
        case EVENT_LBUTTONDOWN:
        {
            //printf("EVENT_LBUTTONDOWN\n") ;
            if(x>=900 && x <1200 && y>=30 && y<=90) {
                outPutUi.DrawButton(0);
                ButtonTypeStatus = 0;
                had_save_flag = false;
                outPutUi.setSaveFlag(had_save_flag);
                outPutUi.CalculateDetectResult();
                imshow(WINDOW_NAME_CHESS_OUTPUT,outPutUi.getOutputMat());//刷新以下
                int key = 0;
                while(1){
                    key = waitKey();
                    if(key == 13  || key == 83 || key == 115){
                        // enter, spacebar, s=save
                        //save
                        cout << "save detect result to DetectConfigFile.ini" <<endl;
                        outPutUi.parseDetectMapToMemory();// to mem
                        update();
                        had_save_flag = true;
                        outPutUi.setSaveFlag(had_save_flag);
                        //imshow(WINDOW_NAME_CHESS,chessmapmat);//刷新以下
                    }else{
                        break;
                    }
                }
            }
        }
            break;
    }
}
void BGFGSegmentor::process_v1(Mat &input, Mat &output) {
    cvtColor(input,gray,COLOR_BGR2GRAY);
    if(background.empty()){
        gray.convertTo(background,CV_32F);
    }
    background.convertTo(backImage,CV_8U);

    in_fun = backImage;
}
void BGFGSegmentor::LocatChessCircle(Mat &input, Mat &output) {
    Mat srcImage = input;
    Mat midImage,dstImage;

}
void BGFGSegmentor::process_v2(Mat &input, Mat &output) {
        Mat srcImage = input;
        Mat midImage,dstImage;

            cvtColor(srcImage,midImage,COLOR_BGR2GRAY);
            GaussianBlur(midImage,midImage,Size(9,9),2,2);


            vector<Vec3f> circles;
            HoughCircles(midImage,circles,HOUGH_GRADIENT,1.5,10,200,100,0,0);

            dstImage = srcImage;
            int Max_radius=-1;
            int Max_i=-1;
            for(size_t i =0; i< circles.size();i++){
                int radius = cvRound(circles[i][2]);
                if(radius > Max_radius){
                    Max_i = i;
                    Max_radius = radius;
                }
            }

            if(fnumber >100){
                Max_B_radius = -1;
                fnumber =0;
            }

            if(circles.size()>0 && Max_i != -1){
                Point center(cvRound(circles[Max_i][0]),cvRound(circles[Max_i][1]));
                int radius = cvRound(circles[Max_i][2]);

                if(Max_B_radius < Max_radius){
                    Max_B_center = center;
                    Max_B_radius = Max_radius;
                }

                // circle(dstImage,center,3,Scalar(0,255,0),-1,8,0);

                // circle(dstImage,center,radius,Scalar(155,50,255),3,8,0);


            }

            if(Max_B_radius != -1 ){
                fnumber++;


                Mat circle1 =Mat::zeros(srcImage.rows,srcImage.cols, CV_8UC3);

                //circle(circle1,Max_B_center,3,Scalar(0,255,0),-1,8,0);
                Rect win =  Rect(Point(Max_B_center.x-Max_B_radius,Max_B_center.y-Max_B_radius),
                                     Point(Max_B_center.x+Max_B_radius,Max_B_center.y+Max_B_radius));

                circle(circle1,Max_B_center,Max_B_radius,Scalar(255,255,255),-1,8,0);

                //dstImage = circle1;
                bitwise_and(srcImage,circle1,dstImage);
                if(!(0 <= win.x && 0 <= win.width && win.x + win.width <= dstImage.cols &&
                           0 <= win.y && 0 <= win.height && win.y + win.height <= dstImage.rows )){

                    if(win.y + win.height > dstImage.rows){
                        win = Rect(win.x,win.y,win.width,abs(dstImage.rows-win.y));
                    } else
                    {
                        printf("error\n");
                    }

                }
                Max_Rect = win;
                //Mat out(dstImage,win);
                //float rad = sqrt(pow(win.height/2,2),pow(win.width/2,2));
                //Max_B_radius = rad;
                //Max_B_center.x = Max_B_radius;
                //Max_B_center.y = Max_B_radius;
                Max_B_theta = get_point_angle(Point(0,0),Max_B_center);
                DrawChessStandLines(dstImage,dstImage);
                in_fun = dstImage;
                dstImage = dstImage;
                output = dstImage;
            }
    try{

        }catch (cv::Exception){
            printf("process_v2 error\n");
            in_fun = input;
        }



}
void BGFGSegmentor::DrawChessStandLines(Mat &input, Mat &output) {
    /*
    circle(input,
           Max_B_center,
           Max_B_radius-5,
           Scalar(55,100,195),//Scalar(255,129,0),
           3,
           LINE_AA);*/

    //Point point_a(Max_B_center.x,Max_B_center.y-Max_B_radius+25);
    Point point_a(input.cols/2+20,input.rows/2-315);
    circle(input,
           point_a,
           10,
           Scalar(55,100,195),//Scalar(255,129,0),
           -1,
           LINE_AA);

    //Point point_d(Max_B_center.x-10,Max_B_center.y+Max_B_radius-50);
    Point point_d(point_a.x-4,point_a.y+608);
    circle(input,
           point_d,
           10,
           Scalar(55,100,195),//Scalar(255,129,0),
           -1,
           LINE_AA);

    int fx= abs(Max_B_center.x - Max_B_radius*cos(33*CV_PI/180));
    int fy= abs(Max_B_center.y - Max_B_radius*sin(33*CV_PI/180));
    fx = abs(point_a.x - 263);
    fy = abs(point_a.y + 146);
    Point point_f(fx,fy);
    circle(input,
           point_f,
           10,
           Scalar(55,100,195),//Scalar(255,129,0),
           -1,
           LINE_AA);

    fx= abs(point_a.x + 259);
    fy= abs(point_a.y + 154);
    Point point_b(fx,fy);
    circle(input,
           point_b,
           10,
           Scalar(55,100,195),//Scalar(255,129,0),
           -1,
           LINE_AA);

    fx= abs(point_a.x + 260);
    fy= abs(point_a.y + 455);
    Point point_c(fx,fy);
    circle(input,
           point_c,
           10,
           Scalar(55,100,195),//Scalar(255,129,0),
           -1,
           LINE_AA);

    fx= abs(point_a.x - 269);
    fy= abs(point_a.y + 451);
    Point point_e(fx,fy);
    circle(input,
           point_e,
           10,
           Scalar(55,100,195),//Scalar(255,129,0),
           -1,
           LINE_AA);

    fx= abs(point_a.x - 88);
    fy= abs(point_a.y + 150);
    Point point_fa(fx,fy);
    circle(input,
           point_fa,
           10,
           Scalar(55,100,195),//Scalar(255,129,0),
           -1,
           LINE_AA);

    fx= abs(point_a.x + 85);
    fy= abs(point_a.y + 152);
    Point point_ba(fx,fy);
    circle(input,
           point_ba,
           10,
           Scalar(55,100,195),//Scalar(255,129,0),
           -1,
           LINE_AA);

    fx= abs(point_a.x - 177);
    fy= abs(point_a.y + 298);
    Point point_fe(fx,fy);
    circle(input,
           point_fe,
           10,
           Scalar(55,100,195),//Scalar(255,129,0),
           -1,
           LINE_AA);

    fx= abs(point_a.x + 172);
    fy= abs(point_a.y + 302);
    Point point_bc(fx,fy);
    circle(input,
           point_bc,
           10,
           Scalar(55,100,195),//Scalar(255,129,0),
           -1,
           LINE_AA);

    fx= abs(point_a.x - 91);
    fy= abs(point_a.y + 451);
    Point point_ed(fx,fy);
    circle(input,
           point_ed,
           10,
           Scalar(55,100,195),//Scalar(255,129,0),
           -1,
           LINE_AA);

    fx= abs(point_a.x + 84);
    fy= abs(point_a.y + 453);
    Point point_cd(fx,fy);
    circle(input,
           point_cd,
           10,
           Scalar(55,100,195),//Scalar(255,129,0),
           -1,
           LINE_AA);

    int thickness = 2;
    int lineType = 8;
    line(input,
         point_a,
         point_e,
         Scalar(255,129,0),
         thickness,
         lineType);
    line(input,
         point_a,
         point_c,
         Scalar(255,129,0),
         thickness,
         lineType);
    line(input,
         point_c,
         point_e,
         Scalar(255,129,0),
         thickness,
         lineType);
    line(input,
         point_f,
         point_b,
         Scalar(255,129,0),
         thickness,
         lineType);
    line(input,
         point_f,
         point_d,
         Scalar(255,129,0),
         thickness,
         lineType);
    line(input,
         point_d,
         point_b,
         Scalar(255,129,0),
         thickness,
         lineType);
    output = input;
}
void BGFGSegmentor::process_v3(Mat &input, Mat &output) {
        Mat srcImage = input;
        Mat midImage,dstImage;
        try{
            Canny(srcImage,midImage,50,200,3);
            cvtColor(midImage,dstImage,COLOR_GRAY2BGR);

            vector<Vec2f> lines;
            HoughLines(midImage,lines,1,CV_PI/180,150,0,0);

            map<int,vector<int>> find_all_same_lines;
            if(lines.size()>0){
                for(size_t i =0 ; i< lines.size();i++){
                    float rho = lines[i][0],theta = lines[i][1];
                    Point pt1,pt2;
                    double a = cos(theta),b=sin(theta);
                    double x0 = a*rho,y0 = b*rho;
                    vector<int> find_same_line;
                    pt1.x = cvRound(x0 + 1000*(-b));
                    pt1.y = cvRound(y0 + 1000*(a));
                    pt2.x = cvRound(x0 - 1000*(-b));
                    pt2.y = cvRound(y0 - 1000*(a));

                    line(dstImage,pt1,pt2,Scalar(55,100,0),1,LINE_AA);

                    for(size_t j =0 ; j< lines.size();j++){
                        if(i == j) continue;
                        float rho_j = lines[j][0],theta_j = lines[j][1];
                        Point pt1_j,pt2_j;
                        double a_j = cos(theta_j),b_j=sin(theta_j);
                        double x0_j = a_j*rho,y0_j = b_j*rho_j;
                        pt1_j.x = cvRound(x0_j + 1000*(-b_j));
                        pt1_j.y = cvRound(y0_j + 1000*(a_j));
                        pt2_j.x = cvRound(x0_j - 1000*(-b_j));
                        pt2_j.y = cvRound(y0_j - 1000*(a_j));

                        int theta_diff = abs(theta-theta_j);
                        int rho_diff = abs(rho_j - rho);
                        if( theta_diff<=5*CV_PI/360  && rho_diff <50){
                            //平行线
                            find_same_line.push_back(j);
                        } else{
                            continue;
                        }
                    }
                    if(find_same_line.size()>1)
                        find_all_same_lines.insert(pair<int,vector<int>>(i,find_same_line));

                }
                {
                    map<int,vector<int>>::iterator iter;
                    list<lines_node>::iterator iter_old;
                    list<lines_node> find_all_same_lines_old;
                    if(fnumber>90){
                        //find_all_same_lines_old.clear();
                    }
                    for(iter = find_all_same_lines.begin(); iter != find_all_same_lines.end(); ++iter){
                        vector<int> v = iter->second;
                        int k = iter->first;
                        int size = v.size();

                        float rho = lines[k][0],theta = lines[k][1];
                        Point pt1,pt2;
                        double a = cos(theta),b=sin(theta);
                        double x0 = a*rho,y0 = b*rho;
                        vector<int> find_same_line;
                        pt1.x = cvRound(x0 + 1000*(-b));
                        pt1.y = cvRound(y0 + 1000*(a));
                        pt2.x = cvRound(x0 - 1000*(-b));
                        pt2.y = cvRound(y0 - 1000*(a));

                        lines_node node;
                        node.theta = theta;
                        node.rho = rho;
                        node.p1 = pt1;
                        node.p2 = pt2;
                        node.count =0;

                        bool find_flag=false;


                        if(v.size() > 1){
                            line(dstImage,pt1,pt2,Scalar(55,100,195),1,LINE_AA);
                            node.count = v.size();
                            find_all_same_lines_old.push_back(node);
                        }
                    }


                    for(iter_old = find_all_same_lines_old.begin(); iter_old != find_all_same_lines_old.end();){
                        lines_node v_old = *iter_old;
                        Point pt1 = v_old.p1;
                        Point pt2 = v_old.p2;
                        //v_old.count >=50;
                        if( v_old.count>4){
                            //line(dstImage,pt1,pt2,Scalar(55,100,195),3,LINE_AA);
                            ++iter_old;
                        } else{
                            ++iter_old;
                        }
                    }


                }
            } else{
                dstImage = input;
            }

            output = dstImage;
        }catch (cv::Exception){
            printf("process_v3 --error!\n");
            output = input;
        }

}


double BGFGSegmentor::get_point_angle(Point pointO, Point pointA) {
    double angle = 0;
    Point point;
    double temp;

    point = Point((pointA.x - pointO.x), (pointA.y - pointO.y));

    if ((0 == point.x) && (0 == point.y)) {
        return 0;
    }

    if (0 == point.x) {
        angle = 90;
        return angle;
    }

    if (0 == point.y) {
        angle = 0;
        return angle;
    }

    temp = fabsf(float(point.y) / float(point.x));
    temp = atan(temp);
    temp = temp * 180 / CV_PI;

    if ((0 < point.x) && (0 < point.y)) {
        angle = 360 - temp;
        return angle;
    }

    if ((0 > point.x) && (0 < point.y)) {
        angle = 360 - (180 - temp);
        return angle;
    }

    if ((0 < point.x) && (0 > point.y)) {
        angle = temp;
        return angle;
    }

    if ((0 > point.x) && (0 > point.y)) {
        angle = 180 - temp;
        return angle;
    }

    printf("sceneDrawing :: getAngle error!");
    return -1;
}
