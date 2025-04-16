//
// Created by wuyou on 25-4-4.
//

#ifndef LOGICSYSTEM_H
#define LOGICSYSTEM_H

#include "Const.h"

class HttpConnection;
typedef std::function<void(std::shared_ptr<HttpConnection>)> HttpHandler;
class LogicSystem : public Singleton<LogicSystem> {
    friend class Singleton<LogicSystem>;
public:
    ~LogicSystem() = default;
    bool HandleGet(const std::string& path, const std::shared_ptr<HttpConnection>& con);
    bool HandlePost(const std::string& path, const std::shared_ptr<HttpConnection>& con);
    void RegGet(const std::string& url, const HttpHandler& handler);
    void RegPost(const std::string& url, const HttpHandler& handler);
private:
    LogicSystem();
    std::map<std::string, HttpHandler> post_handlers;
    std::map<std::string, HttpHandler> get_handlers;
};



#endif //LOGICSYSTEM_H
