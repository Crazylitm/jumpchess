//
// Created by litm on 2020/7/5.
//

#ifndef JUMP0_CHESSNODE_H
#define JUMP0_CHESSNODE_H

#include "ChessInclude.h"

struct ChessNode{
    int mapid=0;
    ChessNode *top_left_next = nullptr;
    ChessNode *top_right_next = nullptr;
    ChessNode *middle_left_next = nullptr;
    ChessNode *middle_right_next = nullptr;
    ChessNode *down_left_next = nullptr;
    ChessNode *down_right_next = nullptr;

    ChessNode(){

    }
    ChessNode(int id){
        mapid = id;
    }
    ChessNode(ChessNode *t1,ChessNode *t2,ChessNode *m1,ChessNode *m2,ChessNode *d1,ChessNode *d2){
        top_left_next= t1;
        top_right_next = t2;
        middle_left_next = m1;
        middle_right_next = m2;
        down_left_next = d1;
        down_right_next = d2;
    }
    void SetNext(ChessNode *node,NextType Type){
        if(node == nullptr) return;
        switch (Type){
            case 1:
                top_left_next = node;
                break;
            case 2:
                top_right_next = node;
                break;
            case 3:
                middle_left_next =node;
                break;
            case 4:
                middle_right_next = node;
                break;
            case 5:
                down_left_next = node;
                break;
            case 6:
                down_right_next = node;
                break;
            default:
                break;
        }
    }
    ChessNode * FindNode(int mapid);
    int insertNode(int mapid,NextType wheretype,ChessNode *head);//top_left 1,top_right 2,middle_left 3,middle_right 4, down_left 5,down_right 6
    void printNode();
    ~ChessNode(){

    }

};

#endif //JUMP0_CHESSNODE_H
