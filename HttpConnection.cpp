//
// Created by wuyou on 25-4-4.
//

#include "HttpConnection.h"
#include "LogicSystem.h"

HttpConnection::HttpConnection(tcp::socket socket) : socket_(std::move(socket)) {
}

void HttpConnection::Start() {
    auto self = shared_from_this();
    http::async_read(socket_, buffer_, request_, [self](const beast::error_code &ec, std::size_t bytes_transferred) {
        try {
            if (ec) {
                std::cout << "http read err is " << ec.message() << std::endl;
                return;
            }

            boost::ignore_unused(bytes_transferred);
            self->HandleRequest();
            self->CheckDeadline();
        } catch (std::exception& e) {
            std::cout << "exception: " << e.what() << std::endl;
        }
    });
}

void HttpConnection::CheckDeadline() {
    auto self = shared_from_this();
    deadline_.async_wait([self](beast::error_code ec) {
        if (!ec) {
            self->socket_.close(ec);
        }
    });
}

void HttpConnection::WriteResponse() {
    auto self = shared_from_this();
    response_.content_length(response_.body().size());
    http::async_write(socket_, response_, [self](beast::error_code ec, std::size_t bytes_transferred) {
        self->socket_.shutdown(tcp::socket::shutdown_send, ec);
        self->deadline_.cancel();
    });
}

void HttpConnection::HandleRequest() {
    // 设置版本
    response_.version(request_.version());
    // 短链接
    response_.keep_alive(false);

    if (request_.method() == http::verb::get) { // 回应GET
        bool success = LogicSystem::GetInstance()->HandleGet(request_.target(), shared_from_this());
        if (!success) {
            response_.result(http::status::not_found);
            response_.set(http::field::content_type, "text/plain");
            beast::ostream(response_.body()) << "url not found\r\n";
            WriteResponse();
            return;
        }
        response_.result(http::status::ok);
        response_.set(http::field::server, "GateServer_CLion");
        WriteResponse();
    }
}
