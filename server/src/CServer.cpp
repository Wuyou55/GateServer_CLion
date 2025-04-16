//
// Created by wuyou on 25-4-4.
//

#include "CServer.h"
#include "HttpConnection.h"

CServer::CServer(boost::asio::io_context &ioc, unsigned short &port)
    : ioc_(ioc), acceptor_(ioc, tcp::endpoint(tcp::v4(), port)), socket_(ioc) {
}

void CServer::start() {
    auto self = shared_from_this();
    acceptor_.async_accept(socket_, [self](beast::error_code ec) {
        try {
            // 出错，放弃这个链接，继续监听其他链接
            if (ec) {
                self->start();
                return;
            }

            // 创建新链接，并且创建HttpConnection类的示例管理这个链接
            std::make_shared<HttpConnection>(std::move(self->socket_))->Start();

            // 继续监听
            self->start();
        } catch (std::exception& e) {
        }
    });
}
