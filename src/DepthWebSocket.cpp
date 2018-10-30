//
//  DepthWebSocket.cpp
//  OKEX
//
//  Created by William Henderson on 23/09/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#include "DepthWebSocket.hpp"
#include <chrono>

DepthWebSocket::DepthWebSocket(std::vector<TradingPair> tradingPairs_): host("real.okex.com"), port("10441"), path("/websocket"), ctx{boost::asio::ssl::context::sslv23}, resolver{ioc}, wss{ioc, ctx}, tradingPairs(tradingPairs_){
    
    ctx.set_verify_mode(boost::asio::ssl::verify_none);
    
    tcp::resolver::results_type results = resolver.resolve(host, port);
    boost::asio::connect(wss.next_layer().next_layer(), results.begin(), results.end());
    
    
    // perform SSL handshake
    wss.next_layer().handshake(boost::asio::ssl::stream_base::client);
    
    // perform WebSocket handshake
    wss.handshake(host, path);
    
    std::cout << "connected" << std::endl;
    
    lob.reserve(tradingPairs.size());
    for(int i=0; i<tradingPairs.size(); i++){
        lob.push_back({LocalOrderBook(), LocalOrderBook()});
    }
    
    //add channels for the depth streams
    for(int i=0; i<tradingPairs.size(); i++){
        wss.write(boost::asio::buffer("{'event':'addChannel','channel':'ok_sub_spot_" + tradingPairs[i].combined + "_depth'}"));
    }
}

using namespace std::chrono;

int DepthWebSocket::read(){
    
    std::ostringstream stream;
    wss.read(buffer);
    stream << boost::beast::buffers(buffer.data());
    buffer.consume(buffer.size());
    std::string incoming = stream.str();
    //std::cout << incoming << std::endl;
    
    unsigned long startPos = incoming.find("channel", 0);
    {
        startPos+=10;
        
        unsigned long endPos = incoming.find("\"",  startPos);
        std::string channel = incoming.substr(startPos, endPos - startPos);
        
        
        if(channel.compare(0, 10, "addChannel") == 0);
        else if(channel.compare(0, 12, "ok_sub_spot_") == 0){
            
            long readStartTime = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
            
            unsigned long startPosC = 12;
            unsigned long endPosC = channel.find("_", channel.find("_", startPosC)+1);
            std::string symbol = channel.substr(startPosC, endPosC - startPosC);
            
            int j=0;
            while(symbol.compare(tradingPairs[j].combined)){
                j++;
            }
            
            std::string nextChar;
            for(int i=0; i<2; i++){
                startPos = incoming.find("[", endPos)+1;
                nextChar = incoming.substr(startPos, 1);
                
                int firstPossiblePos=0;
                while(nextChar.compare(0,1,"[") == 0){
                    
                    startPos+=2;
                    endPos = incoming.find("\"", startPos);
                    double price = std::stod(incoming.substr(startPos, endPos - startPos));
                    startPos = endPos + 3;
                    
                    endPos = incoming.find("\"", startPos);
                    double volume = std::stod(incoming.substr(startPos, endPos - startPos));
                    
                    firstPossiblePos = lob[j][i].update(price, volume, firstPossiblePos) + 1;
                    startPos = endPos + 3;
                    nextChar = incoming.substr(startPos,1);
                }
            }
            startPos = incoming.find("timestamp", endPos) + 11;
            endPos = incoming.find("}", startPos);
            long sentTime = std::stol(incoming.substr(startPos, endPos - startPos));
            
            long readFinishTime = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
            
            std::cout << readStartTime - sentTime << ", " << readFinishTime - readStartTime<< ", " << readFinishTime - sentTime <<std::endl;
            
            return j;
        }
        else{
            std::cout << incoming << std::endl;
        }
        // startPos = incoming.find("channel", endPos);
        
        return -1;
    }
}

Price DepthWebSocket::getBestAsk(int i){
    return lob[i][0].lowPrice(0);
}

Price DepthWebSocket::getBestBid(int i){
    return lob[i][1].highPrice(0);
}

