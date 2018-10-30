//
//  main.cpp
//  OKEX
//
//  Created by William Henderson on 23/09/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#include <iostream>
#include "HTTP.hpp"
#include "DepthWebSocket.hpp"
#include "TradingPair.hpp"
#include "TradesWebSocket.hpp"

int main(int argc, const char * argv[]) {
    HTTP http;
    std::vector<std::string> t = http.getAllTradingPairs();
    DepthWebSocket dws(stringsToTradingPairs(t));
    while(true){
        std::cout << dws.read() << std::endl;
    }
}
