//
// Created by litm on 2023/2/5.
//

#include "Camera_OutPut_UI.h"

void Camera_OutPut_UI::CalculateHadSaveFromConfigParseResult() {
    detectRetMap.clear();
    vector<SaveDetectResultInfo>::iterator iter = SaveDetectPhysical2LogicalInfo.begin();

    for(int i=0; i < result_current.size();i++){
        Rect ARect = result_current[i].box;
        while(iter != SaveDetectPhysical2LogicalInfo.end()){
            Point center  =  iter->circle_center;
            int radius = iter->radius;
            if(ARect.contains(center)){
            }
            iter++;
        }

        // define yolo.h std::vector<std::string> className_jump_chess = { "B", "G", "O", "R", "R2", "W"};
        // 3 = Red   begin marker
        if(result_current[i].id == 3 ){
            // 0,5,1 in CheckersMapLimitCheck.h define int CircleMap[MAX_CHESS][3]={
            DetectChessThreeinfo info(0,5,1,ChessColor(RED));
            DetectResult_UI dResult((Output&)result_current[i],info,15);
            detectRetMap.insert(pair<int,DetectResult_UI>(0,dResult));

            //2 = Orange   end marker
        }else if (result_current[i].id == 2 ){
            // 30,13,17 in CheckersMapLimitCheck.h define int CircleMap[MAX_CHESS][3]={
            DetectChessThreeinfo info(30,13,17,ChessColor(ORANGE));
            DetectResult_UI dResult((Output&)result_current[i],info,15);
            detectRetMap.insert(pair<int,DetectResult_UI>(30,dResult));
        }
    }
}
// this function only call on init posion to save config ;so result_current.size only is size to Red and Orange;
void Camera_OutPut_UI::CalculateResult(){
      if(result_current.size() != 2){
          return;
      }
      if(result_current_no_chess.size()< 0)
          return;
      int Max_Dectect_count = result_current.size() + result_current_no_chess.size();

      cout << "Detect Sum chess counts: " <<Max_Dectect_count <<endl;
/*
      if(Max_Dectect_count != MAX_CHESS)
          return;
          */

      //deal no chess list result
      map<int,vector<Output>> same_y_list;
      std::sort(result_current_no_chess.begin(),result_current_no_chess.end(),mycomp_y());
      for(int j=0; j < result_current_no_chess.size(); j++){

          //find duplicate
          bool find_flag = false;
          for(int z =0 ;find_flag == false && z <=j;z++){
              for(int z2=0; z2 <  same_y_list[z].size();z2++){
                  if( result_current_no_chess[j].box == same_y_list[z][z2].box){
                      //find other one
                      find_flag = true;
                      break;
                  }
              }
          }
          if(find_flag)
              continue;
          same_y_list[j].push_back(result_current_no_chess[j]);
          for(int i =j+1; i <result_current_no_chess.size();i++){
              if(j == i)
                  continue;

              Rect iRect,jRect;
              jRect = result_current_no_chess[j].box;
              iRect = result_current_no_chess[i].box;
              int Max_height = jRect.height > iRect.height ?
                      jRect.height: iRect.height;
              int Max_width = jRect.width > iRect.width ?
                               jRect.width: iRect.width;
              if(abs(jRect.y - iRect.y) < Max_height){
                //same y
                  same_y_list[j].push_back(result_current_no_chess[i]);
              }
              if(abs(jRect.x - iRect.x) < Max_width/2){
                //same x
                cout << "find x same " << endl;
              }

          }
      }
      map<int,vector<Output>>::iterator itor=same_y_list.begin();
      while( itor != same_y_list.end()){
         if(itor->second.empty()) {
             itor = same_y_list.erase(itor);
         } else
             itor++;
      }

      itor=same_y_list.begin();
      map<int,vector<Output>> same_y_list_2;

      while(itor != same_y_list.end()){
          vector<Output> nextlist = itor->second;
          int key = itor->first;
          std::sort(nextlist.begin(),nextlist.end(),mycomp_x());
          same_y_list_2.insert(pair<int,vector<Output>>(key,nextlist));
          itor++;
      }
    //  std:vector<pair<int,vector<Output>>> map_sort_vec(same_y_list.begin(),same_y_list.end());

     // std::sort(map_sort_vec.begin(),map_sort_vec.end(),mycomp_map_y());
    int thickness = 2;
    int lineType = 8;
    same_y_list = same_y_list_2;
    cout << "same_y_list_2 size=" <<same_y_list.size() <<endl;
    itor=same_y_list.begin();
    int i=2;
    // Draw every line info to show
    while (itor !=same_y_list.end()){
         vector<Output> nextlist = itor->second;
         string label ="["+ to_string(i)+"]"+"["+ to_string(nextlist.size())+"]-------------";
         putText(ui_output_mat, label, Point(100,nextlist[0].box.y+20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,0,0), 2);
         for(int j=0; j<nextlist.size()-1;j++){
             int ilen=0;
             ilen =j +1;
             if(ilen > nextlist.size())
                 continue;
             Point left1,right1;
             CalculateRectCenterPosion(nextlist[j].box,left1);
             CalculateRectCenterPosion(nextlist[ilen].box,right1);

             line(ui_output_mat,
                  left1,
                  right1,
                  Scalar(255,129,0),
                  thickness,
                  lineType);
         }
         i++;
         itor++;
     }


    //DetectResult_UI dResult this struct save completed result info, now  step 1 and 2 and 3;
    // step 1 clear the last result
    detectRetMap.clear();

    //sttep 2 add detect result struct
    for(i=0; i < result_current.size();i++){
        // define yolo.h std::vector<std::string> className_jump_chess = { "B", "G", "O", "R", "R2", "W"};
            // 3 = Red   begin marker
           if(result_current[i].id == 3 ){
               // 0,5,1 in CheckersMapLimitCheck.h define int CircleMap[MAX_CHESS][3]={
               DetectChessThreeinfo info(0,5,1,ChessColor(RED));
               DetectResult_UI dResult((Output&)result_current[i],info,15);
               detectRetMap.insert(pair<int,DetectResult_UI>(0,dResult));

               //2 = Orange   end marker
           }else if (result_current[i].id == 2 ){
               // 30,13,17 in CheckersMapLimitCheck.h define int CircleMap[MAX_CHESS][3]={
               DetectChessThreeinfo info(30,13,17,ChessColor(ORANGE));
               DetectResult_UI dResult((Output&)result_current[i],info,15);
               detectRetMap.insert(pair<int,DetectResult_UI>(30,dResult));
           }
    }


    // Draw detect logic chessboard to show
    itor = same_y_list.begin();
    thickness = -1;
    lineType = LINE_AA;
    i=2;
    while(itor != same_y_list.end()){
        map<int,vector<DetectChessThreeinfo>>::iterator find_my_posion_itor= checker.mappingChessPosition.find(i);

        vector<Output> nextlist = itor->second;
        vector<Output>::iterator itor_list = nextlist.begin();
        vector<DetectChessThreeinfo> vecinfo = find_my_posion_itor->second;
        int j=0;
        while(itor_list != nextlist.end()){
            Point centp;
            CalculateRectCenterPosion(itor_list->box,centp);

            //step 3 add detect result struct
            Output ret(itor_list->id,itor_list->confidence,itor_list->box);
            DetectResult_UI dResult(ret,vecinfo[j],15);
            dResult.info.chessColor = ChessColor(NOCHESS);
            detectRetMap.insert(pair<int,DetectResult_UI>(vecinfo[j].index,dResult));

            itor_list++;
            j++;
        }
        itor++;
        i++;
    }
}

Mat Camera_OutPut_UI::getImgRect(Rect one, Rect two,Rect& output) {

    Point center_one,center_two;
    CalculateRectCenterPosion(one,center_one);
    CalculateRectCenterPosion(two,center_two);

    vector<Point>  vecPts;
    vecPts.push_back(one.tl());
    vecPts.push_back(two.tl());
    vecPts.push_back(one.br());
    vecPts.push_back(two.br());

    Mat mask = generateMask(ui_output_mat,vecPts);

    ui_output_mat.copyTo(ui_output_mat,mask);
    return ui_output_mat;




}

/*
 * this function only support six direction of jumpchess
 * from: begin jumpchess
 * to  : end jumpchess
 * only six direction,
 * if spicial posion < six direction,for example top posion,only is 2 diection;
 */
Mat Camera_OutPut_UI::getMask(Rect R_From,Rect R_To){

    Mat mask = Mat::zeros(ui_output_mat.size(), CV_8UC1);
    map<int,set<int>> borders;
    double K;
    int iLen;
    Point a;
    CalculateRectCenterPosion(R_From,a);
    Point b;
    CalculateRectCenterPosion(R_To,b);
    //same point
    if(a.x == b.x && a.y == b.y)
        return mask;
    // too  approach point
    int big_width = (R_From.width > R_To.width ? R_From.width : R_To.width);
    int big_height = (R_From.height > R_To.height ? R_From.height : R_To.height);
    if(abs(a.x - b.x) <= big_width
          && abs(a.y - b.y) <= big_height ){
        return  mask;
    }
    // maybe swap a and b;
    if( a.x - b.x > big_width || a.y - b.y > big_height )
    {
        //swap  a ,b ,
        // is "&&",not "||",because can not happen
        Rect temp = R_From;
        Point temp2 = a;

        R_From = R_To;
        R_To = temp;

        a = b;
        b = temp2;
    }

    int a_width = R_From.width/2;
    int a_hight = R_From.height/2;
    int b_width = R_To.width/2;
    int b_hight = R_To.height/2;
    Point tl,tr,br,bl;
    double K_a2b = -double(a.y - b.y) / (a.x - b.x );
    if(K_a2b > 0){
        tl = Point (R_From.tl());
        tr = Point (R_To.tl());
        br = Point (R_To.br());
        bl = Point (Point(R_From.x+R_From.width,R_From.y+R_From.height));
    } else{
        tl = Point (Point(R_From.x+a_width-7,R_From.y+R_From.height));
        tr = Point (Point(R_From.x+R_From.width,R_From.y+a_hight));
        br = Point (Point(R_To.x+b_width,R_To.y));
        bl = Point (Point(R_To.x,R_To.y+b_hight));
    }


    //1. adjust and find a perfect zone to Mask;
        //Direction : bl to tl extend zone

        if(abs(a.x - b.x)<big_width){
            tl.x = a.x - a_width;
            tl.y = a.y;
            tr.x = a.x + a_width;
            tr.y = a.y;
            bl.x = b.x - b_width;
            bl.y = b.y;
            br.x = b.x + b_width;
            br.y = b.y;
        }else if( abs(a.y - b.y)<big_height){
            tl.x = a.x;
            tl.y = a.y - a_hight;
            tr.x = b.x;
            tr.y = b.y - b_hight;
            bl.x = a.x;
            bl.y = a.y + a_hight;
            br.x = b.x;
            br.y = b.y + b_hight;
        }
    /*
    else{
        if(K_a2b >0)
        {   // when a.y > b.y

                //new tl bl
                K = double(a.y - tl.y) / (a.x - tl.x);
                K = -K;
                if((K_a2b > 0 && K < 0)|| (K_a2b < 0 && K >0 ))
                     K = K_a2b;
                iLen = getDistance(tl,a)/2;
                if(K > 0){
                    tl.y = a.y + cvRound(iLen * K);
                    tl.x = a.x - cvRound( iLen/K);

                    bl.y = b.y + cvRound(iLen * K);
                    bl.x = b.x - cvRound( iLen/K);
                }else{
                    tl.y = a.y + cvRound(iLen * K);
                    tl.x = a.x + cvRound( iLen/K);

                    bl.y = a.y - cvRound(iLen * K);
                    bl.x = a.x - cvRound( iLen/K);
                }
                //new tr br
                K = double(b.y - tr.y) / (b.x - tr.x);
                K = -K;
                if((K_a2b > 0 && K < 0)|| (K_a2b < 0 && K >0 ))
                    K = K_a2b;
                iLen = getDistance(tr,b)/2;
                if(K > 0){
                    tr.y = a.y - cvRound(iLen * K);
                    tr.x = a.x + cvRound( iLen/K);

                    br.y = b.y + cvRound(iLen * K);
                    br.x = b.x - cvRound( iLen/K);
                }else{
                    tr.y = b.y + cvRound(iLen * K);
                    tr.x = b.x + cvRound( iLen/K);

                    br.y = b.y - cvRound(iLen * K);
                    br.x = b.x - cvRound( iLen/K);
            }
        }else{//K_a2b < 0
            //when a.y < b.y
            //new tl bl
            K = double(a.y - bl.y) / (a.x - bl.x);
            K = -K;
            if((K_a2b > 0 && K < 0)|| (K_a2b < 0 && K >0 ))
                K = K_a2b;
            iLen = getDistance(bl,a)/2;
            if(K > 0){
                tl.y = a.y + cvRound(iLen * K);
                tl.x = a.x - cvRound( iLen/K);

                bl.y = b.y + cvRound(iLen * K);
                bl.x = b.x - cvRound( iLen/K);
            }else{
                tl.y = a.y + cvRound(iLen * K);
                tl.x = a.x + cvRound( iLen/K);

                bl.y = a.y - cvRound(iLen * K);
                bl.x = a.x - cvRound( iLen/K);
            }
            //new tr br
            K = double(b.y - tr.y) / (b.x - tr.x);
            K = -K;
            if((K_a2b > 0 && K < 0)|| (K_a2b < 0 && K >0 ))
                K = K_a2b;
            iLen = getDistance(tr,b)/2;
            if(K > 0){
                tr.y = a.y - cvRound(iLen * K);
                tr.x = a.x + cvRound( iLen/K);

                br.y = b.y - cvRound(iLen * K);
                br.x = b.x + cvRound( iLen/K);
            }else{
                tr.y = b.y + cvRound(iLen * K);
                tr.x = b.x + cvRound( iLen/K);

                br.y = b.y - cvRound(iLen * K);
                br.x = b.x - cvRound( iLen/K);
            }
        }


    }
*/
 // avoid out of the windows
    if(tl.x < 0) tl.x = 0;
    if(tl.y < 0) tl.y = 0;
    if(tr.x < 0) tr.x = 0;
    if(tr.y < 0) tr.y = 0;
    if(bl.x < 0) bl.x = 0;
    if(bl.y < 0) bl.y = 0;
    if(br.x < 0) br.x = 0;
    if(br.y < 0) br.y = 0;

    //2. below is new tl,tr,bl,br;
    // tl to tr
    K = double(tl.y-tr.y) / (tl.x - tr.x);
    for (int i = tl.x; i < tr.x; ++i)
        borders[i].insert(cvRound(-K*(tl.x - i) + tl.y));



    // tr to br
    if (tr.x != br.x)
    {
        K = double(tr.y - br.y) / (tr.x - br.x);
        for (int i = tr.x; i < br.x; ++i)
            borders[i].insert(cvRound(-K*(tr.x - i) + tr.y));
    }

    // br to bl
    K = double(br.y - bl.y) / (br.x - bl.x);
    for (int i = bl.x; i < br.x; ++i)
        borders[i].insert(cvRound(-K*(br.x - i) + br.y));

    // bl to tl
    if (bl.x != tl.x)
    {
        K = double(bl.y - tl.y) / (bl.x - tl.x);
        for (int i = tl.x; i < bl.x; ++i)
            borders[i].insert(cvRound(-K*(bl.x - i) + bl.y));
    }

    for (auto it = borders.begin(); it!=borders.end(); ++it)
    {
        if ((*it).second.size() == 2)
            continue;

        std::set<int> newone = { *(*it).second.begin(), *(--(*it).second.end())};
        (*it).second.swap(newone);
    }

    //////////

    int minX = std::min(tl.x, bl.x),
            maxX = std::max(tr.x, br.x),
            minY = std::min(tl.y, tr.y),
            maxY = std::max(bl.y, br.y);

    uchar minZ = std::min(ui_output_mat.at<uchar>(tl), ui_output_mat.at<uchar>(tr));
    minZ = std::min(minZ, ui_output_mat.at<uchar>(bl));
    minZ = std::min(minZ, ui_output_mat.at<uchar>(br));

    for (size_t j= minY; j<maxY; ++j)
    {
        const uchar* pS = ui_output_mat.ptr<uchar>(j);
        uchar* pM = mask.ptr<uchar>(j);
        for (size_t i = minX; i < maxX; ++i)
        {
            // in the region.
            if (*borders[i].begin() < j && j < *(++borders[i].begin()))
            {
                //printf("*borders[i].begin()=%d \n",*borders[i].begin());
                //printf("*(++borders[i].begin())=%d \n",*(++borders[i].begin()));
                pM[i] = pS[i];
            }
        }
    }

    Mat result;
    ui_output_mat.copyTo(result,mask);

    for (auto it = borders.begin(); it != borders.end(); ++it)
    {
        cv::circle(ui_output_mat, cv::Point((*it).first, *(*it).second.begin()), 1, cv::Scalar(255, 255, 0), -1);
        cv::circle(ui_output_mat, cv::Point((*it).first, *(--(*it).second.end())), 1, cv::Scalar(0, 255, 255), -1);

    }

    cv::circle(ui_output_mat, tl, 2, cv::Scalar(0, 0, 255), -1);
    cv::circle(ui_output_mat, tr, 2, cv::Scalar(0, 0, 255), -1);
    cv::circle(ui_output_mat, bl, 2, cv::Scalar(0, 0, 255), -1);
    cv::circle(ui_output_mat, br, 2, cv::Scalar(0, 0, 255), -1);
    return result;

}
int Camera_OutPut_UI::getJumpChessCircle(Rect input) {
    int thickness = -1;
    int lineType = LINE_AA;
    Mat midImage,dstImage;
    Mat roi(ui_output_mat,input);
    cvtColor(roi,midImage,COLOR_BGR2GRAY);
    vector<Vec3f> circles;
    HoughCircles(midImage,circles,HOUGH_GRADIENT,1,5,10,15,10,17);
    dstImage = roi;
    if(temp_debug_flag)
    {
        namedWindow("Debug3");
        imshow("Debug3",midImage);
    }
    for(size_t i =0; i< circles.size();i++){
        int radius = cvRound(circles[i][2]);
        circle(ui_output_mat,
               Point(input.x+circles[i][0], input.y+circles[i][1]),
               radius,
               Scalar(240,240,240),
               thickness,
               lineType);
    }

    return 1;
}
int Camera_OutPut_UI::getJumpChessCircle(Mat input) {
    int thickness = -1;
    int lineType = LINE_AA;
    Mat midImage,dstImage;
    cvtColor(input,midImage,COLOR_BGR2GRAY);
    vector<Vec3f> circles;
    HoughCircles(midImage,circles,HOUGH_GRADIENT,1,5,10,15,10,17);
    dstImage = input;
    if(temp_debug_flag)
    {
        namedWindow("Debug3");
        imshow("Debug3",midImage);
    }
    for(size_t i =0; i< circles.size();i++){
        int radius = cvRound(circles[i][2]);
        circle(ui_output_mat,
               Point(circles[i][0], circles[i][1]),
               radius,
               Scalar(240,240,240),
               thickness,
               lineType);
    }


    return 1;
}
int Camera_OutPut_UI::getJumpChessCircle(int x,int y, Point &circle_center, int &radius) {
    int thickness = -1;
    int lineType = LINE_AA;
    Mat midImage,dstImage;
    int rect_x = x - TOP_X_2_CENTER_DISCTANCE;
    int rect_y = y - TOP_X_2_CENTER_DISCTANCE;
    //chessboard_info_camera.MapChessCalcateMemory.insert(map_key_list[i],)
    Rect circle_rect(rect_x,rect_y,CHESS_CIRCLE_DISCTANCE_MAX,CHESS_CIRCLE_DISCTANCE_MAX);
    if(adjust_flag)
        circle_rect.width = circle_rect.width + ADJUST_LEFT_RIGHT;
    else
        circle_rect.width = circle_rect.width - ADJUST_LEFT_RIGHT;
    Mat roi(ui_output_mat,circle_rect);
    cvtColor(roi,midImage,COLOR_BGR2GRAY);
    vector<Vec3f> circles;
    if(temp_debug_flag)
    {
        namedWindow("Debug2");
        imshow("Debug2",midImage);
    }

    HoughCircles(midImage,circles,HOUGH_GRADIENT,1,5,10,15,10,17);
    dstImage = roi;
    int Max_radius=-1;
    int Max_i=-1;
    for(size_t i =0; i< circles.size();i++){
        int radius = cvRound(circles[i][2]);
        if(radius > Max_radius){
            Max_i = i;
            Max_radius = radius;
        }
    }
    //printf("Max_radius=%d,size=%d\n",Max_radius,circles.size());
    if(Max_radius>0 && Max_i>=0)
    {
        circle_center = Point(rect_x+circles[Max_i][0], rect_y+circles[Max_i][1]);
        radius = Max_radius;
        circle(ui_output_mat,
               circle_center,
               Max_radius,
               Scalar(240,240,240),
               thickness,
               lineType);
        if(temp_debug_flag){
            circle(midImage,
                   Point(circles[Max_i][0], circles[Max_i][1]),
                   Max_radius,
                   Scalar(240,240,240),
                   thickness,
                   lineType);
            namedWindow("Debug");
            imshow("Debug",midImage);
        }


    }
    return 1;
}
void Camera_OutPut_UI::DrawDetectChessOntoBoard() {
    int thickness = 2;
    int lineType = 8;
    thickness = -1;
    lineType = LINE_AA;
    map<int,DetectResult_UI>::iterator itshow = detectRetMap.begin();
    while(itshow != detectRetMap.end()){

        //draw mat
        circle(ui_output_mat,
               itshow->second.circle_center,
               itshow->second.radius,
               Scalar(240,240,240),
               thickness,
               lineType);
        circle(ui_output_mat,
               itshow->second.circle_center,
               itshow->second.radius,
               Scalar(160,129,100),//Scalar(255,129,0),
               2,
               lineType);
        string lable_txt = "("+to_string(itshow->second.info.x)+":"+ to_string(itshow->second.info.y)+")";
        Point txt_pos=Point(itshow->second.circle_center.x-20,itshow->second.circle_center.y);
        putText(ui_output_mat, lable_txt, txt_pos, FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255,0,0), 2);
        itshow++;
    }
}
void Camera_OutPut_UI::rotate(Mat input, Mat &output,vector<Output> result) {
    //{ "B", "G", "O", "R", "R2", "W"};
    int iO=0,iR=0;
    Rect Rbox,Obox;
    for (int i = 0; i < result.size(); i++) {
        switch (result[i].id) {
            case 3:
                iR++;
                Rbox = result[i].box;
                break;
            case 2:
                iO++;
                Obox = result[i].box;
                break;
            default :
                break;
        }
    }
    Point Rcenter,Ocenter;
    CalculateRectCenterPosion(Rbox,Rcenter);
    CalculateRectCenterPosion(Obox,Ocenter);
    //double K = double(Rcenter.y - Ocenter.y) / (Rcenter.x - Ocenter.x);
    if(abs(Rcenter.x-Ocenter.x)>0.0001){
        //double K = double(Rcenter.y - Ocenter.y) / (Rcenter.x - Ocenter.x);
        double Kangle = 90+atan2(double(Rcenter.y - Ocenter.y) ,double (Rcenter.x - Ocenter.x))*180/M_PI;
        //double  Kangle = atan(K)*180/M_PI;
        rotate(input,output,Kangle);
    } else{
        output =input;
    }


}
void Camera_OutPut_UI::rotate(cv::Mat src, cv::Mat &dst, double angle) {
        cv::Size src_sz = src.size();
        cv::Size dst_sz(src_sz.width, src_sz.height);

        cv::Point center = cv::Point(src.cols / 2, src.rows / 2);//旋转中心
        cv::Mat rot_mat = cv::getRotationMatrix2D(center, angle, 1.0);//获得仿射变换矩阵

        cv::warpAffine(src, dst, rot_mat, dst_sz);
}
int  Camera_OutPut_UI::CalculateDetectResult() {
    //by yolo detect Result to Calculate the Chess position
    //must have Blue Chess,Green Chess,Orange Chess,Red Chess,RoseRed(Pink) Chess,White Chess
    //must six chess on board Triangle top position,
    //this function can call to calculate the all chess position on board;
    if(result_current.size() <1)
        return -1;

    bool iB_find= false;
    bool iG_find= false;
    bool iO_find= false;
    bool iR_find= false;
    bool iR2_find= false;
    bool iW_find= false;
    for(int i=0;i < result_current.size();i++){
        int icurX=0,icurY=0;
        if(result_current[i].box.x <=0 ||
           result_current[i].box.y <=0)
            continue;
        Point curCenterP;
        bool flag = CalculateRectCenterPosion(result_current[i].box,curCenterP);
        if(flag){
            if(curCenterP.x <=0 ||
               curCenterP.y <=0)
                continue;
            switch (result_current[i].id) {
                case 0:
                    iB = curCenterP;
                    iB_R=result_current[i].box;
                    iB_find = true;
                    break;
                case 1:
                    iG = curCenterP;
                    iG_R=result_current[i].box;
                    iG_find = true;
                    break;
                case 2:
                    iO = curCenterP;
                    iO_R=result_current[i].box;
                    iO_find = true;
                    break;
                case 3:
                    iR = curCenterP;
                    iR_R=result_current[i].box;
                    iR_find = true;
                    break;
                case 4:
                    iR2 = curCenterP;
                    iR2_R=result_current[i].box;
                    iR2_find = true;
                    break;
                case 5:
                    iW = curCenterP;
                    iW_R=result_current[i].box;
                    iW_find = true;
                    break;
            }
        }
        /*
        printf("result_box:[(%d,%d,%d,%d),[%d,%d]]\n",
               result_current[i].box.x,
               result_current[i].box.y,
               result_current[i].box.width,
               result_current[i].box.height,curCenterP.x,curCenterP.y);
               */

    }

    //call parent function need to deal this error result and show user error msg;
    /*
    if(iB_find == false)
        return 1;
    if(iG_find == false)
        return 2;
    if(iO_find == false)
        return 3;
    if(iR_find == false)
        return 4;
    if(iR2_find == false)
        return 5;
    if(iW_find == false)
        return 6;
    */
    //
    bool iN_find= false;
    if(result_current_no_chess.size() > 0)
        iN_find = true;
    else
        return 7;


    //false to first call to calculate Red and Orange posion to save config
    if(had_save_flag == false){
        //begin to Calculate
        CalculateResult();
        //next deal function
        DrawDetectChessOntoBoard();
    }



    return 0;
}
void Camera_OutPut_UI::CalculateDetectResult_Demo(){
    //by yolo detect Result to Calculate the CCalculateResult_old_v0_1();hess position
    Point iB(10,10),iG(10,10),iO(10,10),iR(10,10),iR2(10,10),iW(10,10);
    int thickness = 2;
    int lineType = 8;
    Point start(10,10);
    if(result_current.size()<0)
            return;

    for(int i=0;i < result_current.size();i++){
            int icurX=0,icurY=0;
            Point curCenterP;
            bool flag = CalculateRectCenterPosion(result_current[i].box,curCenterP);
            if(flag){
                switch (result_current[i].id) {
                    case 0:
                        iB = curCenterP;
                        break;
                    case 1:
                        iG = curCenterP;
                        break;
                    case 2:
                        iO = curCenterP;
                        break;
                    case 3:
                        iR = curCenterP;
                        break;
                    case 4:
                        iR2 = curCenterP;
                        break;
                    case 5:
                        iW = curCenterP;
                        break;
                }
            }
            printf("result_box:[(%d,%d,%d,%d),[%d,%d]]\n",
                   result_current[i].box.x,
                   result_current[i].box.y,
                   result_current[i].box.width,
                   result_current[i].box.height,curCenterP.x,curCenterP.y);
        /*
        if(flag){
            line(ui_output_mat,
                 start,
                 curCenterP,
                 Scalar(255,129,0),
                 thickness,
                 lineType);
        }else{
            Point end(result_current[i].box.x,result_current[i].box.y);
            line(ui_output_mat,
                 start,
                 end,
                 Scalar(255,129,0),
                 thickness,
                 lineType);
        }
         */
    }

    //1
    Point ChessCentP(10,10);
    bool flag = Calculate2PointCentP(iR,iO,ChessCentP);
    if(flag){
        line(ui_output_mat,
             start,
             ChessCentP,
             Scalar(0,255,0),
             thickness,
             lineType);
    }
    //2
    Point ChessCentP2(10,10);
    flag = Calculate2PointCentP(iG,iW,ChessCentP2);
    if(flag){
        int thickness = -1;
        int lineType = LINE_AA;


        circle(ui_output_mat,
               ChessCentP2,
               19,
               Scalar(240,240,240),
               thickness,
               lineType);
        circle(ui_output_mat,
               ChessCentP2,
               19,
               Scalar(160,129,100),//Scalar(255,129,0),
               2,
               lineType);
    }
    //3
    Point ChessCentP3(10,10);
    flag = Calculate2PointCentP(iR2,iR,ChessCentP3);
    if(flag){
        int thickness = -1;
        int lineType = LINE_AA;


        circle(ui_output_mat,
               ChessCentP3,
               19,
               Scalar(240,240,240),
               thickness,
               lineType);
        circle(ui_output_mat,
               ChessCentP3,
               19,
               Scalar(160,129,100),//Scalar(255,129,0),
               2,
               lineType);
    }

    //4
    flag = Calculate2PointCentP(iG,iO,ChessCentP3);
    if(flag){
        int thickness = -1;
        int lineType = LINE_AA;


        circle(ui_output_mat,
               ChessCentP3,
               19,
               Scalar(240,240,240),
               thickness,
               lineType);
        circle(ui_output_mat,
               ChessCentP3,
               19,
               Scalar(160,129,100),//Scalar(255,129,0),
               2,
               lineType);
    }
    //5
    flag = Calculate2PointCentP(iR,iW,ChessCentP3);
    if(flag){
        int thickness = -1;
        int lineType = LINE_AA;


        circle(ui_output_mat,
               ChessCentP3,
               19,
               Scalar(240,240,240),
               thickness,
               lineType);
        circle(ui_output_mat,
               ChessCentP3,
               19,
               Scalar(160,129,100),//Scalar(255,129,0),
               2,
               lineType);
    }
    //6
    flag = Calculate2PointCentP(iG,iB,ChessCentP3);
    if(flag){
        int thickness = -1;
        int lineType = LINE_AA;


        circle(ui_output_mat,
               ChessCentP3,
               19,
               Scalar(240,240,240),
               thickness,
               lineType);
        circle(ui_output_mat,
               ChessCentP3,
               19,
               Scalar(160,129,100),//Scalar(255,129,0),
               2,
               lineType);
    }
    //7
    flag = Calculate2PointCentP(iR2,iW,ChessCentP3);
    if(flag){
        int thickness = -1;
        int lineType = LINE_AA;


        circle(ui_output_mat,
               ChessCentP3,
               19,
               Scalar(240,240,240),
               thickness,
               lineType);
        circle(ui_output_mat,
               ChessCentP3,
               19,
               Scalar(160,129,100),//Scalar(255,129,0),
               2,
               lineType);
    }
    //8
    flag = Calculate2PointCentP(iO,iB,ChessCentP3);
    if(flag){
        int thickness = -1;
        int lineType = LINE_AA;

        /*
        circle(ui_output_mat,
               ChessCentP3,
               19,
               Scalar(240,240,240),
               thickness,
               lineType);
        circle(ui_output_mat,
               ChessCentP3,
               19,
               Scalar(160,129,100),//Scalar(255,129,0),
               2,
               lineType);
               */
        int rect_x = ChessCentP3.x - 19;
        int rect_y = ChessCentP3.y - 19;
        Rect circle_rect(rect_x,rect_y,38,38);
        Mat roi(ui_output_mat,circle_rect);
        namedWindow("Test");
        imshow("Test",roi);

        Mat midImage,dstImage;

        cvtColor(roi,midImage,COLOR_BGR2GRAY);
        //GaussianBlur(midImage,midImage,Size(9,9),2,2);
        namedWindow("Test1");
        imshow("Test1",midImage);

        vector<Vec3f> circles;
        HoughCircles(midImage,circles,HOUGH_GRADIENT,1,5,10,15,10,19);
        namedWindow("Test2");
        imshow("Test2",midImage);

        dstImage = roi;
        int Max_radius=-1;
        int Max_i=-1;
        for(size_t i =0; i< circles.size();i++){
            int radius = cvRound(circles[i][2]);
            circle(midImage,
                   Point(circles[i][0], circles[i][1]),
                   circles[i][2],
                   Scalar(0, 255, 255),
                   2);
            if(radius > Max_radius){
                Max_i = i;
                Max_radius = radius;
            }
        }
        if(Max_radius>0 && Max_i>=0)
        {
            circle(ui_output_mat,
                   Point(rect_x+circles[Max_i][0], rect_y+circles[Max_i][1]),
                   Max_radius,
                   Scalar(240,240,240),
                   thickness,
                   lineType);
        }

        printf("Max_radius=%d,size=%d\n",Max_radius,(int)circles.size());
        waitKey();
    }


}
void Camera_OutPut_UI::DrawBackground() {
    Point dest(10,10);
    Rect rect = Rect(dest.x,dest.y,300,70);
    Mat img = button_check_yolov5;
    Mat img1(img,rect);

    Point dest_point(900,25);
    Rect dest_rect = Rect(dest_point.x,dest_point.y,rect.width,rect.height);
    Mat destimg = ui_output_mat;
    Mat destimg1(destimg,dest_rect);
    cvAdd4cMat_q_static(destimg1,img1,1.0);
    putText(ui_output_mat,"Begin Calculate Detect Result....",Point(910,20),FONT_HERSHEY_COMPLEX,0.5,Scalar(0,255,0));
    //DrawButton(1);
}
void Camera_OutPut_UI::DrawButton(int type) {

        int hight = button_check_yolov5.rows;
        int with  = button_check_yolov5.cols;
        //btton on mouse click downing = type =1 click uping type=0
        //once mouse click run type1 and type 0
        if(type == 1){
            //button on
            Point dest(110,110);

            Rect rect = Rect(dest.x,dest.y,60,60);
            Mat img = button_check_yolov5;
            Mat img1(img,rect);

            Point dest_point(1000,30);
            Rect dest_rect = Rect(dest_point.x,dest_point.y,rect.width,rect.height);
            Mat destimg = ui_output_mat;
            Mat destimg1(destimg,dest_rect);
            cvAdd4cMat_q_static(destimg1,img1,1.0);
        }else if(type ==0 ){
            //button off
            //button on
            Point dest(110,165);
            Rect rect = Rect(dest.x,dest.y,60,60);
            Mat img_off = button_check_yolov5;
            Mat img1_off(img_off,rect);

            Point dest_point(1000,30);
            Rect dest_rect = Rect(dest_point.x,dest_point.y,rect.width,rect.height);
            Mat destimg_off = ui_output_mat;
            Mat destimg1_off(destimg_off,dest_rect);
            cvAdd4cMat_q_static(destimg1_off,img1_off,1.0);
        }


}
// Map one chess have six neighbors chess,so this function can update every chess data;
void Camera_OutPut_UI::ReUpdateCheckersUIMapChessControlMemory() {
    int i=0;
    map<int,list<CircleReturn>> *head = MapChessControlMemory;
    map<int,list<CircleReturn>>::iterator iterMap,iterMap2;
    for(iterMap = head->begin();iterMap != head->end();iterMap++){

        for(iterMap2 = head->begin(); iterMap2 != head->end(); iterMap2++){
            list<CircleReturn>::iterator  iterlist;
            for(iterlist = iterMap2->second.begin();iterlist !=iterMap2->second.end();iterlist++){
                    if((iterMap->second.begin()->CircleMap_i ==iterlist->CircleMap_i) &&
                       (iterMap->second.begin()->Map_x == iterlist->Map_x) &&
                       (iterMap->second.begin()->Map_y == iterlist->Map_y) &&
                       (iterMap->second.begin()->Map_y == iterlist->Map_y)  &&
                       (iterMap->second.begin()->curColor != iterlist->curColor)  ){
                        //same node by curColor defferent
                        iterlist->curColor = iterMap->second.begin()->curColor;
                        iterlist->Down_Chess_flag = iterMap->second.begin()->Down_Chess_flag;
                    }
            }
        }
    }
}
void Camera_OutPut_UI::parseDetectMapToMemory() {
    if(detectRetMap.empty())
        return;
    map<int,DetectResult_UI>::iterator iter = detectRetMap.begin();

    int cur_x,cur_y;
    ChessColor cur_color;
    int i=0;
    map<int,list<CircleReturn>> *head = MapChessControlMemory;
    SaveDetectPhysical2LogicalInfo.clear();
    while(iter != detectRetMap.end()){

        map<int,list<CircleReturn>>::iterator  iterfind = head->find(iter->first);
        if(iterfind != head->end())
        {
            iterfind->second.begin()->CircleMap_i = iter->second.info.index;
            iterfind->second.begin()->Map_x = iter->second.info.x;
            iterfind->second.begin()->Map_y = iter->second.info.y;
            iterfind->second.begin()->curColor= iter->second.info.chessColor;
            iterfind->second.begin()->curPoint;// = iter->second.circle_center;
            if(iter->second.info.chessColor == ChessColor(NOCHESS)){
                iterfind->second.begin()->Down_Chess_flag = true;
                //iterfind->second.begin()->curColor = SHARELIGHTGREEN;
            }
            else
                iterfind->second.begin()->Down_Chess_flag = false;

            //save info init
            SaveDetectResultInfo info;
            info.circle_center = iter->second.circle_center;
            info.radius = iter->second.radius;
            info.index = iter->second.info.index;
            info.x = iter->second.info.x;
            info.y = iter->second.info.y;
            SaveDetectPhysical2LogicalInfo.push_back(info);
        }
        iter++;
        i++;
    }
    ReUpdateCheckersUIMapChessControlMemory();
}
void Camera_OutPut_UI::parseDetectChessPositionToBoardUpdateFromSaveResult( vector<Output>& output,vector<Output>& output_nochess) {
    map<int,list<CircleReturn>> *head = MapChessControlMemory;
    vector<int> colorChessMap_iList;

    for(int i =0; i < output.size() ;i++){
       // map<int,DetectResult_UI>::iterator iter = detectRetMap.begin();
        vector<SaveDetectResultInfo>::iterator iter = SaveDetectPhysical2LogicalInfo.begin();
        Rect ARect = output[i].box;
        while(iter != SaveDetectPhysical2LogicalInfo.end()){

            Point center  =  iter->circle_center;
            int radius = iter->radius;
            if(ARect.contains(center)){
                map<int,list<CircleReturn>>::iterator  iterfind = head->find(iter->index);
                if(iterfind != head->end())
                {
                    iterfind->second.begin()->CircleMap_i = iter->index;
                    iterfind->second.begin()->Map_x = iter->x;
                    iterfind->second.begin()->Map_y = iter->y;
                    iterfind->second.begin()->curColor= ConvertYoloColor2ChessColor(YoloChessColor(output[i].id));
                    //save
                    colorChessMap_iList.push_back(iterfind->second.begin()->CircleMap_i);
                }
                break;
            }
            iter++;
        }
    }

    for(int i =0; i < output_nochess.size() ;i++){
        // map<int,DetectResult_UI>::iterator iter = detectRetMap.begin();
        vector<SaveDetectResultInfo>::iterator iter = SaveDetectPhysical2LogicalInfo.begin();
        Rect ARect = output_nochess[i].box;
        while(iter != SaveDetectPhysical2LogicalInfo.end()){

            Point center  =  iter->circle_center;
            int radius = iter->radius;
            if(ARect.contains(center)){
                map<int,list<CircleReturn>>::iterator  iterfind = head->find(iter->index);
                if(iterfind != head->end())
                {
                    iterfind->second.begin()->CircleMap_i = iter->index;
                    iterfind->second.begin()->Map_x = iter->x;
                    iterfind->second.begin()->Map_y = iter->y;
                    bool find_flag = false;
                    //judge dupliate; jump curColor
                    for(int z=0; z < colorChessMap_iList.size(); z++){
                        if(iterfind->second.begin()->CircleMap_i == colorChessMap_iList[z]){
                            find_flag = true;
                            break;
                        }
                    }
                    if(find_flag == false)
                         iterfind->second.begin()->curColor= ConvertYoloColor2ChessColor(YoloChessColor(Y_NOCHESS));
                }
                break;
            }
            iter++;
        }
    }
    ReUpdateCheckersUIMapChessControlMemory();
}
void Camera_OutPut_UI::diffSaveV1_V2() {
    vector<SaveDetectResultInfo>::iterator iter = SaveDetectPhysical2LogicalInfo.begin();
    vector<SaveDetectResultInfo>::iterator iter_v2;
    for(;iter != SaveDetectPhysical2LogicalInfo.end();iter++){
        iter_v2 = SaveDetectPhysical2LogicalInfo_v2.begin();
        for(;iter_v2 != SaveDetectPhysical2LogicalInfo_v2.end(); iter_v2++){
            if(iter->index == iter_v2->index)
            {
                cout << "find 1" << endl;
            }
        }

    }
}