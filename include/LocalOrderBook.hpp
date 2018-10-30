//
//  LocalOrderBook.hpp
//  OKEX
//
//  Created by William Henderson on 20/09/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#ifndef LocalOrderBook_hpp
#define LocalOrderBook_hpp
#include <stdio.h>
#include <ostream>
//asks ordered lowest to highest, bids ordered highest to lowest

struct Price{
    double price, volume;
};

struct LOBNode{
    double price, volume;
    LOBNode *up, *down;
};

class LocalOrderBook{
    LOBNode *head, *tail;
    
public:
    int size;
    
    LocalOrderBook();
    int update(double price, double volume, int startPos); //returns the position it was inserted; 
    friend std::ostream& operator<< (std::ostream& stream, const LocalOrderBook& lob);
    Price highPrice(int i);
    Price lowPrice(int i);
};


#endif /* LocalOrderBook_hpp */
