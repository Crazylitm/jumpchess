//
// Created by litm on 2020/7/18.
//

#include "BGFGSegmentor.h"

void BGFGSegmentor::process(Mat &input, Mat &output) {
    //process_v1(input,output);
    //process_v2(input,output);
    //input = in_fun;
    //process_v3(in_fun,output);
    DrawChessStandLines(input,output);
    //LocatChessCircle(input,output);

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

    int thickness = 5;
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
