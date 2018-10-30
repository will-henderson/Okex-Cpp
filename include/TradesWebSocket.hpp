//
//  TradesWebSocket.hpp
//  OKEX
//
//  Created by William Henderson on 23/09/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#ifndef TradesWebSocket_hpp
#define TradesWebSocket_hpp

#include <stdio.h>
#include <string>
#include "TradingPair.hpp"
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/ssl.hpp>

using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;

class TradesWebsocket{
    std::vector<TradingPair> tps;
    const std::string host, port, path;
    const std::string apiKey;
    const std::string apiSecret;
    boost::beast::multi_buffer buffer;
    
    boost::asio::io_context ioc;
    boost::asio::ssl::context ctx;
    tcp::resolver resolver;
    websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> wss;
    
    std::string sign(std::vector<std::string> parameters); //formed such that param name, param val, param name, param val, ....
    std::string paramsToString(std::vector<std::string> parameters);
    
    double amount1, amount2;
    
public:
    TradesWebsocket(std::vector<TradingPair> tps, std::string apiKey, std::string apiSecret);
    void printBuffer();
    void read(); //reads buffer and updates prices
    
    //double FOrderBuy(TradingPair& t, double amount);
    //double FOrderSell(TradingPair& t, double amount);
    
    double FOrderBuyMarket(TradingPair& t, double amount);
    double FOrderSellMarket(TradingPair& t, double amount);
};



#endif /* TradesWebSocket_hpp */
