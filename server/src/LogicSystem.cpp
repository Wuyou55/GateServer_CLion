//
// Created by wuyou on 25-4-4.
//

#include "LogicSystem.h"
#include "HttpConnection.h"
#include "VerifyGrpcClient.h"

bool LogicSystem::HandleGet(const std::string& path, const std::shared_ptr<HttpConnection>& con) {
    if (!get_handlers.contains(path)) {
        return false;
    }

    get_handlers[path](con);
    return true;
}

bool LogicSystem::HandlePost(const std::string& path, const std::shared_ptr<HttpConnection>& con) {
    if (!post_handlers.contains(path)) {
        return false;
    }

    post_handlers[path](con);
    return true;
}

void LogicSystem::RegGet(const std::string& url, const HttpHandler& handler) {
    get_handlers.insert(std::make_pair(url, handler));
}

void LogicSystem::RegPost(const std::string& url, const HttpHandler& handler) {
    post_handlers.insert(std::make_pair(url, handler));
}

LogicSystem::LogicSystem() {
    RegGet("/get_test", [this](const std::shared_ptr<HttpConnection> &connection) {
        ostream(connection->response_.body()) << "receive get_test req" << std::endl; // 接收到get_test测试
        int i = 0;
        for (auto &elem: connection->get_params_) {
            i++;
            // 输出可能会乱序， 因为用的是unordered_map
            ostream(connection->response_.body()) << "Param " << i << " : " << elem.first << " : " << elem.second <<
                    std::endl;
        }
    });

    RegPost("/post_test", [](const std::shared_ptr<HttpConnection> &connection) {
        auto body = buffers_to_string(connection->request_.body().data());
        std::cout << "receive body is " << body << std::endl;
        connection->response_.set(http::field::content_type, "text/json");

        Json::Value root, src_root;
        Json::Reader reader;
        bool parse_success = reader.parse(body, src_root);
        if (!parse_success) {
            std::cout << "Failed to parse json data" << std::endl;
            root["error"] = ERROR_JSON;
            std::string json_str = root.toStyledString();
            ostream(connection->response_.body()) << json_str << std::endl;
            return true;
        }

        // 将json数据加载到root中
        auto email = src_root["email"].asString();
        GetVerifyRsp rsp =  VerifyGrpcClient::GetInstance()->GetVerifyCode(email);
        std::cout << "email is " << email << std::endl;
        root["error"] = rsp.error();
        root["email"] = email;
        std::string json_str = root.toStyledString();
        ostream(connection->response_.body()) << json_str;
        return true;
    });
}
