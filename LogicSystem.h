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
    bool HandleGet(std::string path, std::shared_ptr<HttpConnection> con);
    void RegGet(std::string url, HttpHandler handler);
private:
    LogicSystem();
    std::map<std::string, HttpHandler> post_handlers;
    std::map<std::string, HttpHandler> get_handlers;
};



#endif //LOGICSYSTEM_H
