//
// Created by wuyou on 25-4-20.
//

#ifndef REDISMGR_H
#define REDISMGR_H
#include "Const.h"
#include <hiredis/hiredis.h>
#include "RedisConPool.h"
#include "ConfigMgr.h"
class RedisMgr : public Singleton<RedisMgr>
{
    friend class Singleton<RedisMgr>;

public:
    ~RedisMgr();
    bool Get(const std::string& key, std::string& value);
    bool Set(const std::string& key, const std::string& value);
    bool Auth(const std::string& password);
    bool LPush(const std::string& key, const std::string& value);
    bool LPop(const std::string& key, std::string& value);
    bool RPush(const std::string& key, const std::string& value);
    bool RPop(const std::string& key, std::string& value);
    bool HSet(const std::string& key, const std::string& hkey, const std::string& value);
    bool HSet(const char* key, const char* hkey, const char* hvalue, size_t hvaluelen);
    std::string HGet(const std::string& key, const std::string& hkey);
    bool Del(const std::string& key);
    bool ExistsKey(const std::string& key);
    void Close() const;

private:
    RedisMgr();

    std::unique_ptr<RedisConPool> con_pool_;
};


#endif //REDISMGR_H
