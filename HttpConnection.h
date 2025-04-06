//
// Created by wuyou on 25-4-4.
//

#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include "Const.h"

class HttpConnection : public std::enable_shared_from_this<HttpConnection> {
public:
    friend class LogicSystem;
    explicit HttpConnection(tcp::socket socket);
    void Start();
private:
    void CheckDeadline();
    void WriteResponse();
    void HandleRequest();
    tcp::socket socket_;
    beast::flat_buffer buffer_{ 8192 };
    http::request<http::dynamic_body> request_;
    http::response<http::dynamic_body> response_;
    net::steady_timer deadline_{
        socket_.get_executor(), std::chrono::seconds(60)
    };
};


#endif //HTTPCONNECTION_H
