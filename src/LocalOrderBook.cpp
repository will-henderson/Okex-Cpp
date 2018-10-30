//
//  LocalOrderBook.cpp
//  OKEX
//
//  Created by William Henderson on 20/09/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#include "LocalOrderBook.hpp"

LocalOrderBook::LocalOrderBook(): size(0), head(new LOBNode), tail(new LOBNode){
    head->down = tail;
    tail->up = head;
}

int LocalOrderBook::update(double price, double volume, int startPos){ //startPos is the highest position this could possibly be
    LOBNode *fPos = head, *bPos = tail, *mPos;
    int fPosLoc = startPos-1, bPosLoc = size, mPosLoc;
    
    for(int i=0; i<startPos; i++){
        fPos = fPos->down;
    }
    
    while(fPosLoc != bPosLoc-1){
        mPosLoc = (fPosLoc+bPosLoc)/2;
        mPos=fPos;
        int iterations = (bPosLoc-fPosLoc)/2;
        for(int i=0; i<iterations; i++){
            mPos = mPos->down;
        }
        if(price < mPos->price){
            fPosLoc = mPosLoc;
            fPos = mPos;
        }
        else{
            bPosLoc = mPosLoc;
            bPos = mPos;
        }
    }
    
    if(price == fPos->price){
        if(volume == 0){
            mPos->up->down = mPos->down;
            mPos->down->up = mPos->up;
            delete mPos;
            size--;
        }
        else{
            mPos->volume = volume;
        }
        return fPosLoc;
    }
    else if(volume!=0){
        LOBNode* n = new LOBNode{price, volume, fPos, bPos};
        fPos->down = n;
        bPos->up = n;
        size++;
    }
    return fPosLoc+1; //since would be inserted after fPos; 
}

std::ostream& operator<< (std::ostream& stream, const LocalOrderBook& lob){
    
    LOBNode *n = lob.head->down;
    for(int i=0; i<lob.size; i++){
        stream << n->price << n->volume;
        n = n->down;
    }
    
    return stream;
}

Price LocalOrderBook::highPrice(int i){
    LOBNode *n = head->down;
    for(int j=0; j<i; j++){
        n = n->down;
    }
    return {n->price, n->volume};
}

Price LocalOrderBook::lowPrice(int i){
    LOBNode *n = tail->up;
    for(int j=0; j<i; j++){
        n = n->up;
    }
    return {n->price, n->volume};
}
