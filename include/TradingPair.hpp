//
//  TradingPair.hpp
//  OKEX
//
//  Created by William Henderson on 26/08/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#ifndef TradingPair_hpp
#define TradingPair_hpp

#include <stdio.h>
#include <string>
#include <vector>
class TradingPair{
    
public:
    std::string first;
    std::string second;
    std::string combined;
    
    TradingPair(std::string combined_);
    
};

std::vector<TradingPair> stringsToTradingPairs(std::vector<std::string> s);

#endif /* TradingPair_hpp */
