//
//  HTTP.cpp
//  OKEX
//
//  Created by William Henderson on 23/09/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#include "HTTP.hpp"
#include <iostream>

HTTP::HTTP(): host("www.okex.com"), port("443"), ctx{boost::asio::ssl::context::sslv23_client}, resolver{ioc}, stream{ioc, ctx}{
    
    ctx.set_verify_mode(boost::asio::ssl::verify_none);
    
    tcp::resolver::results_type results = resolver.resolve(host, port);
    boost::asio::connect(stream.next_layer(), results.begin(), results.end());
    
    SSL_set_tlsext_host_name(stream.native_handle(), host.c_str());
    
    stream.handshake(ssl::stream_base::client);
    
}

std::vector<std::string> HTTP::getAllTradingPairs(){
    
    std::vector<std::string> toreturn;
    
    http::request<http::string_body> req{http::verb::get, "https://www.okex.com/api/v1/tickers.do", 11};
    req.set(http::field::content_type, "application/x-www-form-urlencoded");
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    
    
    http::write(stream, req);
    
    http::response<http::dynamic_body> res;
    
    std::ostringstream s;
    http::read(stream, buffer, res);
    s << res;
    buffer.consume(buffer.size());
    std::string incoming = s.str();
    
    
    
    unsigned long startPos = incoming.find("symbol", 0);
    while(startPos!=-1){
        startPos += 9;
        unsigned long endPos = incoming.find("\"", startPos);
        toreturn.push_back(incoming.substr(startPos, endPos - startPos));
        startPos = incoming.find("symbol", endPos);
    }
    return toreturn;
    
}
