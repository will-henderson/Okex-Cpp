//
//  DepthWebSocket.hpp
//  OKEX
//
//  Created by William Henderson on 23/09/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#ifndef DepthWebSocket_hpp
#define DepthWebSocket_hpp

#include <stdio.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/ssl.hpp>

#include "LocalOrderBook.hpp"
#include "TradingPair.hpp"

using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;

class DepthWebSocket{
    const std::string host, port, path;
    boost::beast::multi_buffer buffer;
    boost::asio::io_context ioc;
    boost::asio::ssl::context ctx;
    tcp::resolver resolver;
    websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> wss;
    
    std::vector<std::vector<LocalOrderBook> > lob;
    
    std::vector<TradingPair> tradingPairs;
    
public:
    DepthWebSocket(std::vector<TradingPair> tradingPairs_);
    int read(); //returns position of tradingPair read in vector
    bool populated();
    Price getBestAsk(int i);
    Price getBestBid(int i);
};

#endif /* DepthWebSocket_hpp */
