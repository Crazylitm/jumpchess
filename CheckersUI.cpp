//
// Created by litm on 2020/2/2.
//

#include "CheckersUI.h"


void CheckersUI::DrawFiledCircle(Mat img, Point center,int radius) {
    int thickness = -1;
    int lineType = LINE_AA;


    circle(img,
            center,
            radius,
            Scalar(240,240,240),
            thickness,
            lineType);
    circle(img,
           center,
           radius,
           Scalar(160,129,100),//Scalar(255,129,0),
           2,
           lineType);
}
void CheckersUI::DrawEllipse(Mat img, double angle) {
    int thickness = 2;
    int lineType =8;
    ellipse (img,
            Point(WINDOW_WIDTH/2,WINDOW_WIDTH/2),
            Size(WINDOW_WIDTH/4,WINDOW_WIDTH/16),
            angle,
            0,
            360,
            Scalar(255,129,0),
            thickness,
            lineType);
}
void CheckersUI::DrawLine(Mat img, Point start, Point end) {
    int thickness = 2;
    int lineType = 8;
    line(img,
            start,
            end,
            Scalar(255,129,0),
            thickness,
            lineType);

}
void CheckersUI::Line(int x1, int y1, int x2, int y2) {
    DrawLine(chessmapmat,Point(x1,y1),Point(x2,y2));
}
void CheckersUI::Circle(int x, int y, int radius) {
    DrawFiledCircle(chessmapmat,Point(x,y),radius);
}

void CheckersUI::Circle(Point p, int radius) {
    DrawFiledCircle(chessmapmat,p,radius);
}
//这个Circle目的是为了画圆进行选中操作，选中的时候外面有一圈蓝色大圈圈，Type=0的时候是选中，并且暴露之前老的背景
//Type=1的时候就会恢复之前画之前的场景。
void CheckersUI::Circle(Point center, int Type, int radius) {
    int thickness = 2;
    int lineType = LINE_AA;
    static Mat old_chessmapmat;//这个函数内的static变量目的是为了保存老的背景，整个控制在内部完成。但是通过type参数来覆盖。

    if(Type ==0){
        chessmapmat.copyTo(old_chessmapmat);//如果是第一次画选中的圆，外面有个蓝色的圈圈，这个圈圈画上去前需要把之前的旧的背景保存以下，下一次恢复的时候可以使用。
        circle(chessmapmat,
               center,
               radius,
               Scalar(250,50,0),//Scalar(250,50,0)Scalar(0,200,120),
               thickness,
               lineType);
    } else{//Type >0 的时候将保存的之前不带边缘的背景图像覆盖回来
        old_chessmapmat.copyTo(chessmapmat);
        //imshow(WINDOW_NAME_CHESS,chessmapmat);
    }

}
void CheckersUI::Circle(list<Point> *p, int Type, int radius) {
    int thickness = 2;
    int lineType = LINE_AA;
    static Mat old_chessmapmat;
    if(p == nullptr) return;
    if(Type == 0){
        chessmapmat.copyTo(old_chessmapmat);
        if(p->size()>0){
            list<Point>::iterator iter;
            for(iter=p->begin();iter != p->end();++iter){
                Point cur = *iter;
                Point draw_cur = getMapXY(cur.x,cur.y);
                circle(chessmapmat,
                       draw_cur,
                       radius,
                       Scalar(250,50,0),//Scalar(250,50,0)Scalar(0,200,120),
                       thickness,
                       lineType);
            }
        }
    } else{
        old_chessmapmat.copyTo(chessmapmat);
    }

}
void CheckersUI::Triangle(Point p1, Point p2, Point p3,ChessColor color) {


    DrawLine(chessmapmat,p1,p2);
    DrawLine(chessmapmat,p1,p3);
    DrawLine(chessmapmat,p2,p3);

    Point rook_points[1][3];
    rook_points[0][0]  = p1;
    rook_points[0][1]  = p2;
    rook_points[0][2]  = p3;

    const Point* ppt[1] = { rook_points[0] };
    int npt[] = { 3 };

    Scalar fillcolor ;
    switch(color){
        case RED:
            fillcolor = Scalar(0,0,255);
            break;
        case GREEN:
            fillcolor = Scalar(46.3,81.6,47.5);
            break;
        case ROSERED:
            fillcolor = Scalar(150,10,150);
            break;
        case ORANGE:
            fillcolor = Scalar(60,120,200);
            break;
        case WHITE:
            fillcolor = Scalar(200,200,200);
            break;
        case BLUE:
            fillcolor = Scalar(255,76,35);
            break;
        default:
            //SHARELIGHTGREEN;
            fillcolor = Scalar(0,255,0);
            break;
    }
    fillPoly( chessmapmat,
              ppt,
              npt,
              1,
              fillcolor,
              LINE_8 );
}
Point CheckersUI::getMapXY(int x, int y) {

    if(checker.IsLegalPosition(x,y) == false)
        return Point(0,0);

    int x_dis = checker.getDistanceFromRED_5_1_point_X(x,y);
    int y_dis = checker.getDistanceFromRED_5_1_point_Y(x,y);

    return Point(begx+x_dis*xwidth,begy+yhigh*y_dis);
}
void CheckersUI::printChess(Point org,Point dist,ChessColor chess) {
    Mat curmat;
    Mat curmat_bak;
    switch(chess){
        case RED:
            curmat = redchess;
            break;
        case GREEN:
            curmat = greenchess;
            break;
        case ROSERED:
            curmat = roseredchess;
            break;
        case ORANGE:
            curmat = orangechess;
            break;
        case WHITE:
            curmat = whirtechess;
            break;
        case BLUE:
            curmat = bluechess;
            break;
        default:
            curmat = redchess;
    }
    int high = curmat.rows/2;
    int with = curmat.cols/2;


    //如果存在之前位置的背景图片的保存，则需要准备恢复现场的操作。
    if(org.x != 0 && org.y !=0){
        Rect rect_org = Rect(org.x-with,org.y-high,curmat.cols,curmat.rows);
        Mat img_org = chessmapmat_no_chess_org;
        Mat img_org_1(img_org,rect_org);
        Mat img_org_2 = img_org_1.clone();
        Mat img_org_3 =chessmapmat;
        Mat img_org_4(img_org_3,rect_org);
        addWeighted(img_org_4,0,img_org_2,1,0.,img_org_4);
    }

    //针对目标位置，需要先定位出要去落子的目标位置的Rect矩形区域

    Rect rect = Rect(dist.x-with,dist.y-high,curmat.cols,curmat.rows);

    //开始把棋子和背景融合在一起，这里面涉及到棋子圆形和方形之间透明的部分和背景融合的处理
    Mat img1 = chessmapmat;
    Mat img1_t1(img1, rect);//该沟通函数生成一个指向chessmapmat的一个局部rect的指针，后续的修改都会作用到chessmapmat上

    cvAdd4cMat_q(img1_t1,curmat,1.0);//这个函数能解决加载的棋子图片外围透明和背景融合在一起的问题。


}
void CheckersUI::InitChess() {
    int x=0,y=0;
    ChessColor chess;
    for(int i=0;i<MAX_CHESS;i++){
        x = checker.CircleMap[i][0];
        y = checker.CircleMap[i][1];
        chess = (ChessColor )checker.CircleMap[i][2];
        if(checker.IsLegalPosition(x,y)== false ){
            x =0;y=0;
            chess =RED;
        }



        Point p = getMapXY(x,y);
        if(chess != SHARELIGHTGREEN)
            printChess(Point(0,0),p,chess);

        {//for debug
            char text[20];
            sprintf(text,"%d",i);
            putText(chessmapmat,text,Point(p.x-10,p.y+2),1,1,Scalar(0,0,0));
        }
    }

    initMouseParam();//初始化鼠标的回调函数
    initMapList();
}
void CheckersUI::Triangle() {
    /*
    DrawLine(chessmapmat,RED_TOP_5_1_point,ROSERED_TOP_17_13_point);
    DrawLine(chessmapmat,RED_TOP_5_1_point,WHIRTE_TOP_5_13_point);
    DrawLine(chessmapmat,ROSERED_TOP_17_13_point,WHIRTE_TOP_5_13_point);

    DrawLine(chessmapmat,ORANGE_TOP_13_17_point,GREEN_TOP_13_5_point);
    DrawLine(chessmapmat,ORANGE_TOP_13_17_point,BLUE_TOP_1_5_point);
    DrawLine(chessmapmat,GREEN_TOP_13_5_point,BLUE_TOP_1_5_point);
    */

    int x1=0,x2=0,x3=0,y1=0,y2=0,y3=0;
    for(int i=0;i<MAX_CELL;i++){

        x1 = checker.littleTriangleMap[i][0][0];
        y1 = checker.littleTriangleMap[i][0][1];
        x2 = checker.littleTriangleMap[i][1][0];
        y2 = checker.littleTriangleMap[i][1][1];
        x3 = checker.littleTriangleMap[i][2][0];
        y3 = checker.littleTriangleMap[i][2][1];



        Point p1 = getMapXY(x1,y1);
        Point p2 = getMapXY(x2,y2);
        Point p3 = getMapXY(x3,y3);

        if(x1==7 && y1==5){
            printf("debug");
        }
        Triangle(p1,p2,p3,checker.getCircleColor(x1,y1));

        DrawLine(chessmapmat,p1,p2);
        DrawLine(chessmapmat,p1,p3);
        DrawLine(chessmapmat,p2,p3);

        Circle(p1);
        Circle(p2);
        Circle(p3);

    }
    chessmapmat_no_chess_org = chessmapmat.clone();
    InitChess();
}
//png带4通道的图像才能把透明背景显示出来
//https://www.cnblogs.com/huty/p/8517372.html
//函数的逻辑就是把scr的4通道和dst的3通道融合在一起，让4通道里面的透明通道能够和dst完美融合
int CheckersUI::cvAdd4cMat_q(cv::Mat &dst, cv::Mat &scr, double scale)
{
    int dstc= dst.channels();
    int scrc= scr.channels();
    if ( dstc!= 3 ||  scrc!= 4)
    {
        return true;
    }
    if (scale < 0.01)
        return false;
    std::vector<cv::Mat>scr_channels;
    std::vector<cv::Mat>dstt_channels;
    split(scr, scr_channels);
    split(dst, dstt_channels);
    CV_Assert(scr_channels.size() == 4 && dstt_channels.size() == 3);

    if (scale < 1)
    {
        scr_channels[3] *= scale;
        scale = 1;
    }
    for (int i = 0; i < 3; i++)
    {
        dstt_channels[i] = dstt_channels[i].mul(255.0 / scale - scr_channels[3], scale / 255.0);
        dstt_channels[i] += scr_channels[i].mul(scr_channels[3], scale / 255.0);
    }
    merge(dstt_channels, dst);
    return true;
}
void CheckersUI::onMouseHandle_inner(int event, int x, int y, int flags, void *param) {
    static int oneMouseDown=0;
    static Point oneMouseDownPose;
    static list<Point> drawProbablePathList ;
    switch(event){
        case EVENT_MOUSEMOVE:
        {

        }
            break;
        case EVENT_LBUTTONUP:
        {

        }
            break;
        case EVENT_LBUTTONDOWN:
        {
            curMousePos.x = x;
            curMousePos.y = y;
            CircleReturn *pcurChess = getCirclePosXY(x,y);
            Point curChessPoint = pcurChess->curPoint;
            ChessColor curColor = pcurChess->curColor;
            int cur_i = pcurChess->CircleMap_i;
            //下面使用3个static变量保存每次操作的old的值，鼠标第一次down的时候记录一次，第二次down的时候就会使用，并且让鼠标down重新标记为重新开始
            static Point old_cur_Pos(-1,-1);
            static ChessColor  old_curColor;
            static int old_cur_i;


            //鼠标第一次down，通过变量oneMouseDown==0来判断是否是鼠标第一次按下Down
            //如果当前落地下地方的颜色不是空棋子的地方，就可以进入这个函数了
            //SHARELIGHTGREEN 被使用为标记是否为空的标记了，代码中总是以它为标记，每次移动，棋子空余下来的位置也会被标记为SHARELIGHTGREEN的
            if(curChessPoint.x !=-1 && curChessPoint.y != -1 && oneMouseDown==0 && curColor != SHARELIGHTGREEN){
                //这个Circle第一次画圆，Type=0，就是操作完成就是显示为选中颜色
                Circle(curChessPoint,0,CHESS_RADIUS+2);
                //if(oneMouseDown==0)
                //保存这次鼠标选中的值为old值，下一次再次鼠标down的时候可以操作使用。
                {
                    old_curColor = pcurChess->curColor;
                    old_cur_i = cur_i;
                    old_cur_Pos = curChessPoint;
                }
                oneMouseDownPose = curChessPoint;

                oneMouseDown++;//标记这个变量目前已经选中了，下一次再进入就不再🥇🥇这段代码，而是金else另外2个分支

                drawProbablePathList.clear();
                //显示当前所有可能的跳动路径。
                control->ProbableJumpPathALLShow(curChessPoint,pcurChess->CircleMap_i,0,&drawProbablePathList);
                Circle(&drawProbablePathList,0,CHESS_RADIUS+2);
                imshow(WINDOW_NAME_CHESS,chessmapmat);//刷新以下
                //如果是第二次鼠标down下来，并且目标位置是空白位置则进入
                //CanJumpFun函数判断这次Jump Chess跳动是否合法，如果不合法，则不允许跳动。
            } else if(curChessPoint.x !=-1 && curChessPoint.y != -1 && oneMouseDown >0 && curColor == SHARELIGHTGREEN
               && control->CanJumpFun(old_cur_Pos,curChessPoint)){

                Circle(oneMouseDownPose, 1, CHESS_RADIUS + 2);//将之前保存的点击前的背景恢复以下
                control->ProbableJumpPathALLShow(old_cur_Pos,pcurChess->CircleMap_i,1);//擦除之前一次点击鼠标down保留的棋盘上显示的所有可能的跳跃路径。
                Circle(&drawProbablePathList,1,CHESS_RADIUS+2);

                printChess(oneMouseDownPose, curChessPoint,  old_curColor);//然后在目的位置打印棋子
                imshow(WINDOW_NAME_CHESS, chessmapmat);//刷新以下
                updateCircleMap(old_cur_i, old_cur_Pos,SHARELIGHTGREEN);//更新CircleMap的数据将数组中old位置标记为空，因为棋子已经移动走了
                updateCircleMap(cur_i, curChessPoint, old_curColor);//在目标位置CircleMap的数据数组的位置更新为新的棋子的数据，这样表示棋子已经🐚落进去了。
                old_curColor = curColor;
                old_cur_Pos = curChessPoint;
                old_cur_i = cur_i;
                oneMouseDown = 0;//重置鼠标down的flag，已经两次点击了，下一次就是一个新的棋子要移动，这次已经移动完毕。


                //当第二次单击的地方是当其已经选择中的棋子在的位置，则需要取消选中状态。
            } else if(curChessPoint.x !=-1 && curChessPoint.y != -1 && oneMouseDown >0 && curChessPoint == old_cur_Pos){

                Circle(oneMouseDownPose, 1, CHESS_RADIUS + 2);
                control->ProbableJumpPathALLShow(old_cur_Pos,1);//擦除之前一次点击鼠标down保留的棋盘上显示的所有可能的跳跃路径。

                old_curColor = curColor;
                old_cur_Pos = curChessPoint;
                old_cur_i = cur_i;
                oneMouseDown = 0;
                imshow(WINDOW_NAME_CHESS, chessmapmat);//刷新以下
            }
            delete(pcurChess);
        }
            break;
    }
}
void CheckersUI::updateCircleMap(int CircleMap_i,Point p, ChessColor color) {
    if(CircleMap_i<0 || CircleMap_i>MAX_CHESS){
        return;
    }
    checker.CircleMap[CircleMap_i][2]= color;

    /*
    map<int, list<CircleReturn>>::iterator iter_org;
    iter_org = MapChessControlMemory.find(CircleMap_i);
    CircleReturn *cur_node= nullptr,*returan_node = nullptr;
    if (iter_org != MapChessControlMemory.end()) {
        list<CircleReturn> curlist = iter_org->second;
        list<CircleReturn>::iterator iterlist;
        for (iterlist = curlist.begin(); iterlist != curlist.end(); iterlist++) {
            cur_node= &(*iterlist);
            if (cur_node->nextType == SELF){
                (*iterlist).curColor = color;
                break;
            }
        }
    }
    */
}
CircleReturn* CheckersUI::getCirclePosXY(int x, int y) {
    Point retpoint(-1,-1);
    ChessColor retColor=RED;
    CircleReturn *ret= new CircleReturn(retpoint,retColor);
    if(x<0 || y <0) return  ret;
    if(x > WINDOW_WIDTH || y > WINDOW_HIGHT) return  ret;

    int map_x=0,map_y=0;
    ChessColor chess;
    for(int i=0;i<MAX_CHESS;i++){
        map_x = checker.CircleMap[i][0];
        map_y = checker.CircleMap[i][1];
        chess = (ChessColor )checker.CircleMap[i][2];
        if(checker.IsLegalPosition(map_x,map_y)== false ){
            x =-1;y=-1;
            chess =RED;
            return ret;
        }
        Point p = getMapXY(map_x,map_y);


        if(getDistance(p,Point(x,y))<=CHESS_RADIUS){

            ret->curColor=chess;
            ret->curPoint=p;
            ret->CircleMap_i = i;
            return ret;
        }
    }
    return ret;

}

void CheckersUI::initMouseParam() {

    setMouseCallback(WINDOW_NAME_CHESS,onMouseHandle,(void*)this);
}
/*
 *
 * 该函数目的是为了维护一个chess棋子周围有多少个棋子，以及和棋盘之间的关系，这样在后续
 *搜索和update的时候，可以通过搜索该数据结构，能够快速找到如何跳跃棋子
 * */
void CheckersUI::initMapList() {
    //control->initMapList();

    int cur_x,cur_y;
    ChessColor cur_color;
    for(int i=0; i<MAX_CHESS;i++){
        cur_x = checker.CircleMap[i][0];
        cur_y = checker.CircleMap[i][1];
        cur_color = (ChessColor)checker.CircleMap[i][2];
        CircleReturn  *node = new CircleReturn();
        node->CircleMap_i = i;
        node->curColor = cur_color;
        node->Map_x = cur_x;// logic Map x
        node->Map_y = cur_y;// logic Map y
        node->curPoint = getMapXY(cur_x,cur_y); // real Map x,y
        node->nextType =SELF;
        list<CircleReturn> *listnode = new list<CircleReturn>();
        listnode->push_back(*node);  //不添加自己到map里面了

        //default one chess have max six linked chess. this loop find the real link chess.
        list<Point> *p_neighbourNode = NULL;
        p_neighbourNode = checker.getChessNeighbourList(Point(cur_x,cur_y));
        list<Point>::iterator it;
        if(p_neighbourNode != NULL ){
            for( it = p_neighbourNode->begin();it != p_neighbourNode->end();it++){
                    Point p = *it;
                    NextType  curType;
                    if(node->Map_x == p.x && node->Map_y -1 == p.y){
                        curType = TLEFT;
                    }
                    if(node->Map_x-1 == p.x && node->Map_y -1 == p.y){
                        curType = TRIGHT;
                    }
                    if(node->Map_x + 1 == p.x && node->Map_y == p.y){
                        curType = MLEFT;
                    }
                    if(node->Map_x-1 == p.x && node->Map_y == p.y){
                        curType = MRIGHT;
                    }
                    if(node->Map_x+1 == p.x && node->Map_y +1 == p.y){
                        curType = DLEFT;
                    }
                    if(node->Map_x == p.x && node->Map_y +1 == p.y){
                        curType = DRIGHT;
                    }
                    for(int j=0; j< MAX_CHESS;j++){
                        if(checker.CircleMap[j][0] == p.x &&
                           checker.CircleMap[j][1] == p.y){
                            CircleReturn  *p_node = new CircleReturn();
                            p_node->CircleMap_i = j;
                            p_node->curColor = (ChessColor)checker.CircleMap[j][2];
                            p_node->Map_x = checker.CircleMap[j][0];// logic Map x
                            p_node->Map_y = checker.CircleMap[j][1];// logic Map y
                            p_node->curPoint = getMapXY( checker.CircleMap[j][0], checker.CircleMap[j][1]); // real Map x,y
                            p_node->nextType = curType;
                            listnode->push_back(*p_node);
                            break;
                        }
                    }
            }
        }

        MapChessControlMemory.insert(pair<int,list<CircleReturn>>(i,*listnode));
        //printf("-------[%d]\n",i);
    }
    printChessMap();
    initChessNodeGraph();
    head->printNode();
    control->init(head);
}
void CheckersUI::initChessNodeGraph(){
    map<int, list<CircleReturn>>::iterator iter_map;
    ChessNode *cur_chessnode = nullptr;
    vector<int> find_st;
    vector<int> g;
    set<int> visited;

    for(iter_map = MapChessControlMemory.begin();iter_map != MapChessControlMemory.end();iter_map++) {
        list<CircleReturn> chess_list;
        list<CircleReturn>::iterator iter_list;
        chess_list = iter_map->second;
        int map_id = iter_map->first;

        if (head == nullptr) {
            head = new ChessNode(map_id);
            cur_chessnode = head;
        } else {
            cur_chessnode = head->FindNode(map_id);
            if (cur_chessnode == nullptr) {
                g.push_back(map_id);
                continue;
            }
        }

        for (iter_list = chess_list.begin(); iter_list != chess_list.end(); iter_list++) {
            CircleReturn node = *iter_list;

            switch (node.nextType) {
                case TLEFT:
                    //printf("->[%d-(%d,%d)-Top-Left]", node.CircleMap_i, node.Map_x, node.Map_y);
                    cur_chessnode->insertNode(node.CircleMap_i, TLEFT,head);
                    break;
                case TRIGHT:
                    //printf("->[%d-(%d,%d)-Top-Right]", node.CircleMap_i, node.Map_x, node.Map_y);
                    cur_chessnode->insertNode(node.CircleMap_i, TRIGHT,head);
                    break;
                case MLEFT:
                    //printf("->[%d-(%d,%d)-Middle-Left]", node.CircleMap_i, node.Map_x, node.Map_y);
                    cur_chessnode->insertNode(node.CircleMap_i, MLEFT,head);
                    break;
                case MRIGHT:
                    //printf("->[%d-(%d,%d)-Middle-Right]", node.CircleMap_i, node.Map_x, node.Map_y);
                    cur_chessnode->insertNode(node.CircleMap_i, MRIGHT,head);
                    break;
                case DLEFT:
                    //printf("->[%d-(%d,%d)-Down-Left]", node.CircleMap_i, node.Map_x, node.Map_y);
                    cur_chessnode->insertNode(node.CircleMap_i, DLEFT,head);
                    break;
                case DRIGHT:
                    //printf("->[%d-(%d,%d)-Down-Right]", node.CircleMap_i, node.Map_x, node.Map_y);
                    cur_chessnode->insertNode(node.CircleMap_i, DRIGHT,head);
                    break;
            }
        }
    }
    int id =0;
    map<int, list<CircleReturn>>::iterator iter_org;
    while(g.size() >0){
        id = g[0];
        g.erase(g.begin());//访问一次就清除，避免重复访问
        cur_chessnode = head->FindNode(id);
        if(cur_chessnode == nullptr){
            if(g.size() < 1  ) break; //如果已经是最后一个，则退出while循环
            g.push_back(id);//如果不是最后一个，则让前面一个先findnode
            continue;
        } else {
            iter_org = MapChessControlMemory.find(id);
            //如果找到了，对这个list对象进行再次迭代
            if (iter_org != MapChessControlMemory.end()) {
                list<CircleReturn> curlist = iter_org->second;
                list<CircleReturn>::iterator iterlist;

                for (iterlist = curlist.begin(); iterlist != curlist.end(); iterlist++) {
                    CircleReturn node = *iterlist;
                    switch (node.nextType) {
                        case TLEFT:
                            //printf("->[%d-(%d,%d)-Top-Left]", node.CircleMap_i, node.Map_x, node.Map_y);
                            cur_chessnode->insertNode(node.CircleMap_i, TLEFT,head);
                            break;
                        case TRIGHT:
                            //printf("->[%d-(%d,%d)-Top-Right]", node.CircleMap_i, node.Map_x, node.Map_y);
                            cur_chessnode->insertNode(node.CircleMap_i, TRIGHT,head);
                            break;
                        case MLEFT:
                            //printf("->[%d-(%d,%d)-Middle-Left]", node.CircleMap_i, node.Map_x, node.Map_y);
                            cur_chessnode->insertNode(node.CircleMap_i, MLEFT,head);
                            break;
                        case MRIGHT:
                            //printf("->[%d-(%d,%d)-Middle-Right]", node.CircleMap_i, node.Map_x, node.Map_y);
                            cur_chessnode->insertNode(node.CircleMap_i, MRIGHT,head);
                            break;
                        case DLEFT:
                            //printf("->[%d-(%d,%d)-Down-Left]", node.CircleMap_i, node.Map_x, node.Map_y);
                            cur_chessnode->insertNode(node.CircleMap_i, DLEFT,head);
                            break;
                        case DRIGHT:
                            //printf("->[%d-(%d,%d)-Down-Right]", node.CircleMap_i, node.Map_x, node.Map_y);
                            cur_chessnode->insertNode(node.CircleMap_i, DRIGHT,head);
                            break;
                    }
                }
            }
        }
    }
    if(g.size()>0)
        printf("initChessNodeGraph---g.count >0 %d ",g[0]);
}
void CheckersUI::printChessMap() {
    map<int, list<CircleReturn>>::iterator iter_map;
    printf("\n");
    for(iter_map = MapChessControlMemory.begin();iter_map != MapChessControlMemory.end();iter_map++){
        list<CircleReturn>  chess_list;
        list<CircleReturn>::iterator iter_list;
        chess_list = iter_map->second;
        int map_id = iter_map->first;
        int x = checker.CircleMap[map_id][0];
        int y = checker.CircleMap[map_id][1];
        printf("[%d-(%d,%d)]---",map_id,x,y);
        for(iter_list = chess_list.begin();iter_list != chess_list.end();iter_list++){
            CircleReturn node = *iter_list;

            switch (node.nextType){
                case TLEFT:
                    printf("->[%d-(%d,%d)-Top-Left]",node.CircleMap_i,node.Map_x,node.Map_y);
                    break;
                case TRIGHT:
                    printf("->[%d-(%d,%d)-Top-Right]",node.CircleMap_i,node.Map_x,node.Map_y);
                    break;
                case MLEFT:
                    printf("->[%d-(%d,%d)-Middle-Left]",node.CircleMap_i,node.Map_x,node.Map_y);
                    break;
                case MRIGHT:
                    printf("->[%d-(%d,%d)-Middle-Right]",node.CircleMap_i,node.Map_x,node.Map_y);
                    break;
                case DLEFT:
                    printf("->[%d-(%d,%d)-Down-Left]",node.CircleMap_i,node.Map_x,node.Map_y);
                    break;
                case DRIGHT:
                    printf("->[%d-(%d,%d)-Down-Right]",node.CircleMap_i,node.Map_x,node.Map_y);
                    break;
                default:
                    printf("->[%d-(%d,%d)-Self]",node.CircleMap_i,node.Map_x,node.Map_y);
                    break;
            }

        }
        printf("\n");
    }
}