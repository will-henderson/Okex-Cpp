//
//  TradesWebSocket.cpp
//  OKEX
//
//  Created by William Henderson on 23/09/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#include "TradesWebSocket.hpp"

TradesWebsocket::TradesWebsocket(std::vector<TradingPair> tradingPairs, std::string apiKey_, std::string apiSecret_): host("real.okex.com"), port("10441"), path("/websocket"), apiKey(apiKey_), apiSecret(apiSecret_), ctx{boost::asio::ssl::context::sslv23}, resolver{ioc}, wss{ioc, ctx}, tps(tradingPairs){
    
    ctx.set_verify_mode(boost::asio::ssl::verify_none);
    tcp::resolver::results_type results = resolver.resolve(host, port);
    boost::asio::connect(wss.next_layer().next_layer(), results.begin(), results.end());
    
    
    // perform SSL handshake
    wss.next_layer().handshake(boost::asio::ssl::stream_base::client);
    
    // perform WebSocket handshake
    wss.handshake(host, path);
    
    //add channels for the currency prices

    //login to the account
    std::vector<std::string> parameters;
    parameters.push_back("api_key");
    parameters.push_back(apiKey);
    const std::string loginRequest = "{'event':'login','parameters':{'api_key':'"+ apiKey + "','sign':'" + sign(parameters) + "'}}";
    wss.write(boost::asio::buffer(loginRequest));
    
    //subscribe to ok_sub_spot_X_order transaction data interface, and ok_sub_spot_X_balance account information interface.
    
    for(int i=0; i<tradingPairs.size(); i++){
        wss.write(boost::asio::buffer("{'event':'addChannel','channel':'ok_sub_spot_" + tps[i].combined + "_order'}"));
        wss.write(boost::asio::buffer("{'event':'addChannel','channel':'ok_sub_spot_" + tps[i].combined + "_balace'}"));
    }

    //get user account info
    const std::string aiRequest = "{'event':'addChannel','channel':'ok_spot_userinfo','parameters':{'api_key':'"+ apiKey + "','sign':'" + sign(parameters) + "'}}";
    wss.write(boost::asio::buffer(aiRequest));
}

#include <openssl/md5.h>

std::string TradesWebsocket::sign(std::vector<std::string> parameters){ //returns signature of parameters
    std::string tbSigned;
    for(int i=0; i<parameters.size(); i+=2){
        tbSigned += parameters[i] + "=" + parameters[i+1] + "&";
    }
    tbSigned += "secret_key=" + apiSecret;
    unsigned char* digest = MD5(reinterpret_cast<unsigned char*>(&tbSigned[0]), tbSigned.size(), NULL);
    
    char mdString[33];
    for(int i = 0; i < 16; i++)
        sprintf(&mdString[i*2], "%02X", (unsigned int)digest[i]);
    
    return std::string(mdString);
}

void TradesWebsocket::read(){
    std::ostringstream stream;
    wss.read(buffer);
    stream << boost::beast::buffers(buffer.data());
    std::cout << boost::beast::buffers(buffer.data());
    buffer.consume(buffer.size());
    std::string incoming = stream.str();
    
    //check channel
    unsigned long startPos = incoming.find("channel", 0) + 10;
    unsigned long endPos = incoming.find("\"",  startPos);
    std::string channel = incoming.substr(startPos, endPos - startPos);
    
    bool found = false;

    for(int i=0; i<tps.size(); i++){
        std::string comparison = "ok_sub_spot_" + tps[i].combined + "_balance";
        if(channel.compare(0, comparison.length(), comparison) == 0){
            startPos = incoming.find(tps[i].first, endPos);
            if(startPos==-1){
                startPos = incoming.find(tps[i].second, endPos) + tps[i].second.length() + 2;
                endPos = incoming.find("}",  startPos);
                amount2 = std::stod(incoming.substr(startPos, endPos - startPos));
            }
            else{
                startPos += tps[i].first.length() + 2;
                endPos = incoming.find("}",  startPos);
                amount1 = std::stod(incoming.substr(startPos, endPos - startPos));
            }
            found = true;
            break;
        }
    }
    if(channel.compare(0, 13,"ok_spot_order") == 0){
        startPos = incoming.find("error_code", startPos);
        if(startPos != -1){
            startPos += 12;
            endPos = incoming.find("}", startPos);
            throw std::stoi(incoming.substr(startPos, endPos - startPos));
        }
    }
    else{
        std::cout << incoming << std::endl;
    }
    
    
}


double TradesWebsocket::FOrderBuyMarket(TradingPair& t, double amount){
    //place order
    std::vector<std::string> parameters1 {"api_key", apiKey, "price", std::to_string(amount), "symbol", t.combined, "type", "buy_market"};
    wss.write(boost::asio::buffer("{'event':'addChannel','channel':'ok_spot_order','parameters':{'api_key':'" + apiKey + "','sign':'" + sign(parameters1) + "','symbol':'" + t.combined + "','type':'buy_market','price':'" + std::to_string(amount) + "'}}"));
    
    amount1=0;
    while(amount1==0){
        read();
    }
    
    return amount1;
}

double TradesWebsocket::FOrderSellMarket(TradingPair& t, double amount){
    //place order
    std::vector<std::string> parameters1 {"amount", std::to_string(amount),"api_key", apiKey, "symbol", t.combined, "type", "sell_market"};
    wss.write(boost::asio::buffer("{'event':'addChannel','channel':'ok_spot_order','parameters':{'api_key':'" + apiKey + "','sign':'" + sign(parameters1) + "','symbol':'" + t.combined + "','type':'sell_market','amount':'" + std::to_string(amount) + "'}}"));
    
    amount2=0;
    while(amount2==0){
        read();
    }
    
    return amount2;
}

