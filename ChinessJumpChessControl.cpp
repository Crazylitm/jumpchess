//
// Created by litm on 2020/3/15.
//

#include <stack>
#include "ChinessJumpChessControl.h"
void ChinessJumpChessControl::init(ChessNode *p) {
    head = p;
}
bool ChinessJumpChessControl::CanJumpFun(Point source, Point dest) {
    return true;
}
bool ChinessJumpChessControl::ProbableJumpPathALLShow(Point source,int Map_i,int Type,list<Point> *p_get) {
    //ui->checker.CircleMap[][]
    if(Map_i<0 || Map_i>MAX_CHESS)return false;
    static Mat old_chessmapmat;
    int map_x=0,map_y=0;
    ChessColor chess;
    if(Type ==0 ){
        chessmapmat.copyTo(old_chessmapmat);

        map_x = checker->CircleMap[Map_i][0];
        map_y = checker->CircleMap[Map_i][1];
        chess = (ChessColor )checker->CircleMap[Map_i][2];
        if(p_get != nullptr && chess !=SHARELIGHTGREEN) {
            map<string,list<CircleReturn>> * p = FindPathList(Map_i);
            if(p != nullptr){
                map<string, list<CircleReturn>>::iterator iter_map;
                for(iter_map = p->begin();iter_map != p->end();iter_map++){
                    list<CircleReturn>  chess_list;
                    list<CircleReturn>::iterator iter_list;
                    chess_list = iter_map->second;
                    string map_id = iter_map->first;
                    printf("{%s}---",map_id.c_str());
                    for(iter_list = chess_list.begin();iter_list != chess_list.end();iter_list++) {
                        CircleReturn node = *iter_list;
                        p_get->push_back(Point(node.Map_x,node.Map_y));
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
            //draw circle

        }
    } else{
        old_chessmapmat.copyTo(chessmapmat);
    }
    return true;
}
map<string,list<CircleReturn>> * ChinessJumpChessControl::FindPathList(int Map_id) {

   if(head == nullptr) return nullptr;
   ChessNode *f = head->FindNode(Map_id);
   if(f == nullptr) return nullptr;
   int x = checker->CircleMap[Map_id][0];
   int y = checker->CircleMap[Map_id][1];
   ChessColor  c = (ChessColor)checker->CircleMap[Map_id][2];
   if(c == SHARELIGHTGREEN) return nullptr;// 当前不是棋子，所以不需要跳动

    map<string,list<CircleReturn>>  *result_map = new map<string,list<CircleReturn>>();
    vector<int> find_st;
    vector<ChessNode*> g;
    set<int> visited;
    ChessNode *cur= nullptr,*second_cur= nullptr;
    int id=0;
    g.push_back(f);
    visited.insert(Map_id);

    int jump_chess_neighbor_id[7];
    stringstream stringid_make;
    stringid_make << "("<<Map_id << ")";
    while (g.size()>0){

        cur = g[0];
        id = cur->mapid;

        jump_chess_neighbor_id[0] = -1;
        jump_chess_neighbor_id[1] = -1;
        jump_chess_neighbor_id[2] = -1;
        jump_chess_neighbor_id[3] = -1;
        jump_chess_neighbor_id[4] = -1;
        jump_chess_neighbor_id[5] = -1;
        jump_chess_neighbor_id[6] = -1;
        if(cur->top_left_next != nullptr){
            jump_chess_neighbor_id[TLEFT] = cur->top_left_next->mapid;
        }
        if(cur->top_right_next != nullptr){
            jump_chess_neighbor_id[TRIGHT] = cur->top_right_next->mapid;
        }
        if(cur->middle_left_next != nullptr){
            jump_chess_neighbor_id[MLEFT] = cur->middle_left_next->mapid;
        }
        if(cur->middle_right_next != nullptr){
            jump_chess_neighbor_id[MRIGHT] = cur->middle_right_next->mapid;
        }
        if(cur->down_left_next != nullptr){
            jump_chess_neighbor_id[DLEFT] = cur->down_left_next->mapid;
        }
        if(cur->down_right_next != nullptr){
            jump_chess_neighbor_id[DRIGHT] = cur->down_right_next->mapid;
        }
        jump_chess_neighbor_id[SELF] = id;


        g.erase(g.begin());
        find_st.push_back(id);
        int count = 7;
        NextType  cur_type=SELF;
        while(count-->0){
            switch(count){
                case TLEFT:
                    second_cur = cur->top_left_next;
                    cur_type = TLEFT;
                    break;
                case TRIGHT:
                    second_cur = cur->top_right_next;
                    cur_type = TRIGHT;
                    break;
                case MLEFT:
                    second_cur = cur->middle_left_next;
                    cur_type = MLEFT;
                    break;
                case MRIGHT:
                    second_cur = cur->middle_right_next;
                    cur_type = MRIGHT;
                    break;
                case DLEFT:
                    second_cur = cur->down_left_next;
                    cur_type = DLEFT;
                    break;
                case DRIGHT:
                    second_cur = cur->down_right_next;
                    cur_type = DRIGHT;
                    break;
                default:
                    second_cur = nullptr;
                    cur_type = SELF;
                    break;
            }

            if(second_cur != nullptr){
                ChessNode *nextcur = second_cur;
                int nextid = nextcur->mapid;
                vector<ChessNode*> sec_g;
                set<int> sec_visited;
                list<int> his_list;
                ChessNode *sec_cur= nullptr;
                int sec_id=0;
                sec_g.push_back(nextcur);
                sec_visited.insert(nextid);
                his_list.push_back(nextid);
                ChessNode *th_cur = nullptr;

                while (sec_g.size()>0){
                    sec_cur = sec_g[0];
                    sec_id = sec_cur->mapid;
                    sec_g.erase(sec_g.begin());

                    switch(cur_type){
                        case TLEFT:
                                th_cur = sec_cur->top_left_next;
                            break;
                        case TRIGHT:
                                th_cur = sec_cur->top_right_next;
                            break;
                        case MLEFT:
                                th_cur = sec_cur->middle_left_next;
                            break;
                        case MRIGHT:
                                th_cur = sec_cur->middle_right_next;
                            break;
                        case DLEFT:
                                th_cur = sec_cur->down_left_next;
                            break;
                        case DRIGHT:
                                th_cur = sec_cur->down_right_next;
                            break;
                        default:
                            break;
                    }
                    if( th_cur != nullptr){
                        if(sec_visited.count(th_cur->mapid)==0){
                            sec_g.push_back(th_cur);
                            sec_visited.insert(th_cur->mapid);
                            his_list.push_back(th_cur->mapid);
                        }
                    }
                }
                //finded some list to deal
                //(1) id (2) sec_id (3) th_cur->mapid
                list<int>::iterator iter;
                CircleReturn *p = nullptr;
                ChessColor SEC_LIST[20]; //max =13 over 13 is error
                int MAP_ID[20];
                for(int j=0;j<20;j++){
                    SEC_LIST[j] = NOCHESS;
                    MAP_ID[j] =-1;
                }
                int i=0;
                for(iter = his_list.begin(); iter != his_list.end(); ++iter){
                    p = GetNodeInfo(*iter);
                    if(p != nullptr){
                        if(i>20) break;
                        SEC_LIST[i] = p->curColor;
                        MAP_ID[i++] = p->CircleMap_i;
                    }
                }
                i++;// include himself
                list<int> *rec = GetMatchList(SEC_LIST,i);
                if(rec->size()>0) {
                    list<CircleReturn> *listnode = new list<CircleReturn>();
                    for (iter = rec->begin(); iter != rec->end(); ++iter) {
                        int id_test = MAP_ID[*iter];
                        set<int>::iterator it_temp0 = sec_visited.find(id_test);
                        CircleReturn *p0 = GetNodeInfo(*it_temp0);
                        ///
                        bool Flag =false;
                        if(Map_id != id_test) {//second search ,cannot find neighbor chess,just find next jump chess dump;
                            for (int i = 0; i < 7; i++) {
                                if (p0->CircleMap_i == jump_chess_neighbor_id[i] && Map_id != id) {
                                        Flag = true;
                                        break;
                                }
                            }
                        }
                        if(Flag == true )
                        {
                            continue;
                        } else{
                            if(visited.count(p0->CircleMap_i)==0){
                                Flag = false;
                                for (int i = 0; i < 7; i++) {
                                    if (p0->CircleMap_i == jump_chess_neighbor_id[i]) {
                                        Flag = true;
                                        break;
                                    }
                                }
                                if(Flag == false) {
                                    ChessNode *t = head->FindNode(p0->CircleMap_i);
                                    if(t != nullptr){
                                        g.push_back(t);
                                        visited.insert(p0->CircleMap_i);
                                    }
                                }

                            } else{
                                continue;
                            }
                        }

                        static int i=1000;
                        i++;
                        listnode->push_back(*p0);
                        stringstream tempstr;
                        tempstr << stringid_make.str();
                        tempstr <<  "["<<i <<"]";
                        string t = tempstr.str();
                        result_map->insert(pair<string, list<CircleReturn>>(t, *listnode));
                    }
                }
            }//if


        }//while(6)

    }//while(g>0)

    return  result_map;


}
list<int>* ChinessJumpChessControl::GetMatchList(ChessColor *sec, int sec_count) {
    if(sec_count == 0 || sec == nullptr ) return nullptr;
    stack<int> sec_st_0_firt,sec_st_1;
    list<int> *result_list=new list<int>();
    int deal_sec[20];
    for(int i =0; i<20;i++)
        deal_sec[i]=-1;
    //transform the sec[]
    deal_sec[0] = 0 ; //add the jump chess to firt stop,other chesses add back
    for(int i =0; i< sec_count; i++){
        if(sec[i] == SHARELIGHTGREEN) {
            deal_sec[i+1] = 0;
        }else if(sec[i] == -1 ){
            deal_sec[i+1] = -1;
        } else{
            deal_sec[i+1] = 1;
        }
    }
    for(int i =0 ;i < sec_count+1;i++){// sec_count+1  is include myself
        if(deal_sec[i]==0){
            if(sec_st_1.empty() == false){
                sec_st_0_firt.pop();
                if(sec_st_0_firt.empty() == true){
                    sec_st_1.pop();// match one
                    result_list->push_back(i-1);// record this location
                    sec_st_0_firt.push(0);// add first shelf to begin

                }
            } else{
                sec_st_0_firt.push(0);
            }
        } else if(deal_sec[i] == 1){
                if(sec_st_1.empty()){
                    sec_st_1.push(1);
                } else{
                    break;// meet 1 by 1 ,so break search ;
                }

        } else{
            break;//end
        }
    }
    if(result_list->size()==0){
        if(deal_sec[1] == 0){
            result_list->push_back(0);
        } else if(deal_sec[1] ==1){
            if(sec_count>1){
                if(deal_sec[2] == 0){
                    result_list->push_back(1);
                }
            }
        }
    }else{
        if(deal_sec[1] == 0){
            result_list->push_back(0);
        }
    }
    return result_list;
}
CircleReturn* ChinessJumpChessControl::GetNodeInfo(int Map_id) {
    map<int, list<CircleReturn>>::iterator iter_org;
    iter_org = MapChessControlMemory->find(Map_id);
    CircleReturn *cur_node= nullptr,*returan_node = nullptr;
    if (iter_org != MapChessControlMemory->end()) {
        list<CircleReturn> curlist = iter_org->second;
        list<CircleReturn>::iterator iterlist;

        for (iterlist = curlist.begin(); iterlist != curlist.end(); iterlist++) {
            cur_node= &(*iterlist);
            if (cur_node->nextType == SELF){
                returan_node = cur_node;
                returan_node->curColor = (ChessColor)checker->CircleMap[cur_node->CircleMap_i][2];//update current map status of Color
                break;
            }
        }
    }
    return returan_node;
}
void ChinessJumpChessControl::FindPathList(int Map_id,map<int,list<CircleReturn>> &circlemap)  {
    int map_x=0,map_y=0;
    ChessColor  map_Color=NOCHESS;
    vector<int> find_st;
    vector<int> g;
    set<int> visited;




    g.push_back(Map_id);//while循环控制条件，里面保存当前要访问的下一个mapid
    visited.insert(Map_id);//保存访问过的mapid的list，为后面判断是否重复拜访作为判断条件
    while(g.size()>0) {
        int id =g[0];
        map<int, list<CircleReturn>>::iterator iter_org;
        map_x = checker->CircleMap[id][0];
        map_y = checker->CircleMap[id][1];
        map_Color = (ChessColor)checker->CircleMap[Map_id][2];

        iter_org = MapChessControlMemory->find(id);
        g.erase(g.begin());//访问一次就清除，避免重复访问
        find_st.push_back(id);//保存每一个访问的mapid的list

        if (iter_org != MapChessControlMemory->end()) {
            list<CircleReturn> curlist = iter_org->second;
            list<CircleReturn>::iterator iterlist;
            int cur_i1 = 0;
            int map_x1 = 0, map_y1 = 0;
            ChessColor curcolor = NOCHESS;

            for (iterlist = curlist.begin(); iterlist != curlist.end(); iterlist++) {
                CircleReturn cur_node = *iterlist;
                if (cur_node.Map_x == map_x && cur_node.Map_y == map_y)
                    continue;
                map_x1 = cur_node.Map_x;
                map_y1 = cur_node.Map_y;
                cur_i1 = cur_node.CircleMap_i;
                curcolor = (ChessColor) cur_node.curColor;

                //find_st.push_back(cur_i1);
                //判断是否已经访问过这个mapid，如果没有访问过，则继续循环执行放在g里面
                //并且标记现在已经拜访国这个mapid
                /*
                if(visited.count(cur_i1) == 0){
                    g.push_back(cur_i1);
                    visited.insert(cur_i1);
                }*/

                //寻找下一跳的方向
                map<int,list<CircleReturn>>::iterator iter2;
                iter2 = MapChessControlMemory->find(cur_i1);

                if(iter2 != MapChessControlMemory->end()) {
                    list<CircleReturn> curlist2 = iter2->second;
                    list<CircleReturn>::iterator iterlist2;
                    int cur_i2 = 0;
                    int map_x2 = 0, map_y2 = 0;
                    ChessColor curcolor2 = NOCHESS;
                    for (iterlist2 = curlist2.begin(); iterlist2 != curlist2.end(); iterlist2++) {
                        CircleReturn cur_node2 = *iterlist2;
                        if (cur_node2.Map_x == map_x1 && cur_node2.Map_y == map_y1)
                            continue;

                        map_x2 = cur_node2.Map_x;
                        map_y2 = cur_node2.Map_y;
                        cur_i2 = cur_node.CircleMap_i;
                        curcolor2 = (ChessColor) cur_node2.curColor;


                        //find_st.push_back(cur_i2);
                        //判断是否已经访问过这个mapid，如果没有访问过，则继续循环执行放在g里面
                        //并且标记现在已经拜访国这个mapid
                        if(visited.count(cur_i2) == 0){
                            g.push_back(cur_i2);
                            visited.insert(cur_i2);
                        }
                        //
                        bool find_line_flag = false;
                        if(map_x == map_x1 && map_x == map_x2 ){
                            //3个位置都在x坐标相同的这一行
                            find_line_flag = true;
                        }
                        if(find_line_flag== false && (map_y == map_y1 && map_y == map_y2)){
                            //3个作为都在y坐标相同的这一行
                            find_line_flag = true;
                        }
                        int ax = abs(map_x - map_x1);
                        int cx = abs(map_x1 - map_x2);
                        int ay = abs(map_y - map_y1);
                        int cy = abs(map_y1 - map_y2);
                        if(find_line_flag==false && (ax ==1 && cx == 1 && ay == 1 && cy ==1)){
                            //判断是否是除了x，y另外一种直线的倾斜方式，3个点是否在一条线上
                            find_line_flag =true;
                        }
                        if(find_line_flag == true){
                            //三个点在一条线上
                            if(map_Color != SHARELIGHTGREEN){
                                if(curcolor != SHARELIGHTGREEN){
                                    //前2个连续是两个棋子
                                    if(curcolor2 !=SHARELIGHTGREEN){
                                        //第一个是棋子，第二个是棋子，第三个也是棋子
                                        //stop search
                                        //find_st.push_back(cur_i2);
                                        //判断是否已经访问过这个mapid，如果没有访问过，则继续循环执行放在g里面
                                        //并且标记现在已经拜访国这个mapid
                                        continue;
                                    } else{
                                        //第一个是棋子，第二个是棋子，第三个是空格
                                        //stop search
                                        continue;
                                    }
                                } else{
                                    if(curcolor2 !=SHARELIGHTGREEN){
                                        //第一个是棋子，第二个空格，第三个是棋子
                                        //continue search
                                    } else{
                                        //第一个是棋子，第二个是空格，第三个也是空格
                                        //continue search

                                    }
                                }
                            } else{
                                if(curcolor != SHARELIGHTGREEN){
                                    if(curcolor2 !=SHARELIGHTGREEN){
                                        //第一个是空格，二个是棋子，第三个也是棋子
                                        //stop search
                                        continue;
                                    } else{
                                        //第一个是空格，第二个是棋子，第三个是空格
                                        //continue search

                                    }
                                } else{
                                    if(curcolor2 !=SHARELIGHTGREEN){
                                        //第一个是空格，第二个空格，第三个是棋子
                                        //continue search
                                    } else{
                                        //第一个是空格，第二个是空格，第三个也是空格
                                        //continue search

                                    }
                                }
                            }


                        } else{
                            //三个不在一条线上这个搜索方向要改一下
                            continue;
                            //  if()
                        }
                    }
                }

            }

        }
    }


}
