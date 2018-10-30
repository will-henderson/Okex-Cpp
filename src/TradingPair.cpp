//
//  TradingPair.cpp
//  OKEX
//
//  Created by William Henderson on 26/08/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#include "TradingPair.hpp"

TradingPair::TradingPair(std::string combined_){
    combined = combined_;
    int secondStart  = combined.find("_");
    
    first = combined.substr(0,secondStart);
    second = combined.substr(secondStart+1, combined.length());
}

std::vector<TradingPair> stringsToTradingPairs(std::vector<std::string> s){
    std::vector<TradingPair> toreturn;
    for(int i=0; i<s.size(); i++){
        toreturn.push_back(TradingPair(s[i]));
    }
    return toreturn;
}
