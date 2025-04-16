//
// Created by wuyou on 25-4-4.
//

#ifndef CSERVER_H
#define CSERVER_H

#include "Const.h"

class CServer : public std::enable_shared_from_this<CServer> {
public:
    CServer(boost::asio::io_context &ioc, unsigned short &port);

    void start();

private:
    net::io_context &ioc_;
    tcp::acceptor acceptor_;
    tcp::socket socket_;
};


#endif //CSERVER_H
