//
// Created by wuyou on 25-4-4.
//

#include "LogicSystem.h"
#include "HttpConnection.h"

bool LogicSystem::HandleGet(std::string path, std::shared_ptr<HttpConnection> con) {
    if (get_handlers.contains(path)) {
        return false;
    }

    get_handlers[path](con);
    return true;
}

void LogicSystem::RegGet(std::string url, HttpHandler handler) {
    get_handlers.insert(std::make_pair(url, handler));
}

LogicSystem::LogicSystem() {
    RegGet("/get_test", [this](const std::shared_ptr<HttpConnection> &connection) {
        beast::ostream(connection->response_.body()) << "receive get_test req"; // 接收到get_test测试
    });
}
