//
// Created by litm on 2020/7/5.
//

#include "ChessNode.h"
void ChessNode::printNode() {
    vector<ChessNode*> g;
    set<int> visited;
    ChessNode * curptr = this,*nextcurptr=this;
    int cur_mapid=0,next_id=0;
    g.push_back(this);
    visited.insert(this->mapid);
    cout << "\n";
    printf("->[%d]\n",this->mapid);
    while(g.size()>0){
        int id = g[0]->mapid;
        cur_mapid = id;
        curptr = g[0];
        g.erase(g.begin());


        if(curptr->top_left_next != nullptr){
            nextcurptr = curptr->top_left_next;
            next_id = nextcurptr->mapid;

            if(visited.count(next_id) == 0){
                g.push_back(nextcurptr);
                visited.insert(next_id);
                printf("->[%d]\n",next_id);
            }
        }
        if(curptr->top_right_next != nullptr){
            nextcurptr = curptr->top_right_next;
            next_id = nextcurptr->mapid;


            if(visited.count(next_id) == 0){
                g.push_back(nextcurptr);
                visited.insert(next_id);
                printf("->[%d]\n",next_id);
            }
        }
        if(curptr->middle_right_next != nullptr){
            nextcurptr = curptr->middle_right_next;
            next_id = nextcurptr->mapid;


            if(visited.count(next_id) == 0){
                g.push_back(nextcurptr);
                visited.insert(next_id);
                printf("->[%d]\n",next_id);
            }
        }
        if(curptr->middle_left_next != nullptr){
            nextcurptr = curptr->middle_left_next;
            next_id = nextcurptr->mapid;


            if(visited.count(next_id) == 0){
                g.push_back(nextcurptr);
                visited.insert(next_id);
                printf("->[%d]\n",next_id);
            }
        }
        if(curptr->down_left_next != nullptr){
            nextcurptr = curptr->down_left_next;
            next_id = nextcurptr->mapid;


            if(visited.count(next_id) == 0){
                g.push_back(nextcurptr);
                visited.insert(next_id);
                printf("->[%d]\n",next_id);
            }
        }
        if(curptr->down_right_next != nullptr){
            nextcurptr = curptr->down_right_next;
            next_id = nextcurptr->mapid;


            if(visited.count(next_id) == 0){
                g.push_back(nextcurptr);
                visited.insert(next_id);
                printf("->[%d]\n",next_id);
            }
        }
    }

}
ChessNode* ChessNode::FindNode(int mapid) {

    if(this->mapid == mapid) return this;

    vector<int> find_st;
    vector<ChessNode*> g;
    set<int> visited;
    ChessNode * curptr = this ,*nextcurptr=this;
    int cur_mapid=0;
    bool find_flag=0;

    g.push_back(this);//while循环控制条件，里面保存当前要访问的下一个mapid
    visited.insert(this->mapid);//保存访问过的mapid的list，为后面判断是否重复拜访作为判断条件
    while (g.size() >0){
        int id =g[0]->mapid;
        curptr = g[0];
        g.erase(g.begin());//访问一次就清除，避免重复访问
        find_st.push_back(id);//保存每一个访问的mapid的list
        if(curptr->top_left_next != nullptr){
            nextcurptr = curptr->top_left_next;
            cur_mapid = nextcurptr->mapid;
            if(cur_mapid == mapid)
            {
                find_flag =1;
                curptr = nextcurptr;
                break;
            }

            if(visited.count(cur_mapid) == 0){
                g.push_back(nextcurptr);
                visited.insert(cur_mapid);
            }
        }
        if(curptr->top_right_next != nullptr){
            nextcurptr = curptr->top_right_next;
            cur_mapid = nextcurptr->mapid;
            if(cur_mapid == mapid)
            {
                find_flag =2;
                curptr = nextcurptr;
                break;
            }

            if(visited.count(cur_mapid) == 0){
                g.push_back(nextcurptr);
                visited.insert(cur_mapid);
            }
        }
        if(curptr->middle_right_next != nullptr){
            nextcurptr = curptr->middle_right_next;
            cur_mapid = nextcurptr->mapid;
            if(cur_mapid == mapid)
            {
                find_flag =3;
                curptr = nextcurptr;
                break;
            }

            if(visited.count(cur_mapid) == 0){
                g.push_back(nextcurptr);
                visited.insert(cur_mapid);
            }
        }
        if(curptr->middle_left_next != nullptr){
            nextcurptr = curptr->middle_left_next;
            cur_mapid = nextcurptr->mapid;
            if(cur_mapid == mapid)
            {
                find_flag =4;
                curptr = nextcurptr;
                break;
            }

            if(visited.count(cur_mapid) == 0){
                g.push_back(nextcurptr);
                visited.insert(cur_mapid);
            }
        }
        if(curptr->down_left_next != nullptr){
            nextcurptr = curptr->down_left_next;
            cur_mapid = nextcurptr->mapid;

            if(cur_mapid == mapid)
            {
                find_flag =5;
                curptr = nextcurptr;
                break;
            }

            if(visited.count(cur_mapid) == 0){
                g.push_back(nextcurptr);
                visited.insert(cur_mapid);
            }
        }
        if(curptr->down_right_next != nullptr){
            nextcurptr = curptr->down_right_next;
            cur_mapid = nextcurptr->mapid;
            if(cur_mapid == mapid)
            {
                find_flag =6;
                curptr = nextcurptr;
                break;
            }

            if(visited.count(cur_mapid) == 0){
                g.push_back(nextcurptr);
                visited.insert(cur_mapid);
            }
        }


    }

    if(find_flag > 0)
        return curptr;
    else
        return nullptr;
}
/*
 * mapid: insert chessmap id
 * wheretype: 1,2,3,4,5,6, t1,t2,m1,m2,d1,d2
 * return: 0 ok , -1 exist, -2 wheretype=error
 *
 * */
int ChessNode::insertNode(int mapid,NextType wheretype,ChessNode * head) {

    if(head == nullptr) return -3;
    ChessNode *p;
    p = head->FindNode(mapid);
    if( p == nullptr){
        p = new ChessNode();
        p->mapid = mapid;
    }


    switch (wheretype){
        case TLEFT://top_left
            if(this->top_left_next == nullptr){
                this->top_left_next = p;
            } else{
                return -1;
            }
            break;
        case TRIGHT://top_right
            if(this->top_right_next == nullptr){
                this->top_right_next = p;
            } else{
                return -1;
            }
            break;
        case MLEFT://middle_left
            if(this->middle_left_next == nullptr){
                this->middle_left_next = p;
            } else{
                return -1;
            }
            break;
        case MRIGHT://middle_right
            if(this->middle_right_next == nullptr){
                this->middle_right_next = p;
            } else{
                return -1;
            }
            break;
        case DLEFT://down_left
            if(this->down_left_next == nullptr){
                this->down_left_next = p;
            } else{
                return -1;
            }
            break;
        case DRIGHT://down_right
            if(this->down_right_next == nullptr){
                this->down_right_next = p;
            } else{
                return -1;
            }
            break;
        default:
            return  -2;
    }

    return 0;
}