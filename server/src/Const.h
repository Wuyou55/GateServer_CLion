//
// Created by wuyou on 25-4-4.
//

#ifndef CONST_H
#define CONST_H

#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
#include "Singleton.h"
#include <atomic>
#include <queue>

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http; // from <boost/beast/http.hpp>
namespace net = boost::asio; // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

enum ErrorCode {
    SUCCESS = 0,
    ERROR_JSON = 1001,
    RPCFailed = 1002,
};

#endif //CONST_H
