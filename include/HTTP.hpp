//
//  HTTP.hpp
//  OKEX
//
//  Created by William Henderson on 23/09/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#ifndef HTTP_hpp
#define HTTP_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/streambuf.hpp>

using tcp = boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
namespace http = boost::beast::http;

class HTTP{
    
    const std::string host, port;
    boost::beast::flat_buffer buffer;
    
    boost::asio::io_context ioc;
    boost::asio::ssl::context ctx;
    tcp::resolver resolver;
    ssl::stream<tcp::socket> stream;
    
public:
    HTTP();
    std::vector<std::string> getAllTradingPairs();
    
};
#endif /* HTTP_hpp */
