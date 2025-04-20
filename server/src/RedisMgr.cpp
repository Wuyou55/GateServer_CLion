//
// Created by wuyou on 25-4-20.
//

#include "RedisMgr.h"

RedisMgr::~RedisMgr()
{
    Close();
}

bool RedisMgr::Get(const std::string& key, std::string& value)
{
    auto connect = con_pool_->getConnection();
    if (connect == nullptr)
    {
        return false;
    }
    auto reply = static_cast<redisReply*>(redisCommand(connect, "GET %s", key.c_str()));
    if (reply == nullptr)
    {
        std::cout << "[ GET " << key << " ] failed" << std::endl;
        freeReplyObject(reply);
        return false;
    }
    if (reply->type != REDIS_REPLY_STRING)
    {
        std::cout << "[ GET " << key << " ] failed (type)" << std::endl;
        freeReplyObject(reply);
        return false;
    }
    value = reply->str;
    freeReplyObject(reply);

    std::cout << "[ GET " << key << " ] success" << std::endl;
    con_pool_->returnConnection(connect);
    return true;
}

bool RedisMgr::Set(const std::string& key, const std::string& value)
{
    auto connect = con_pool_->getConnection();
    auto reply = static_cast<redisReply*>(redisCommand(connect, "SET %s %s", key.c_str(), value.c_str()));

    if (reply == nullptr)
    {
        std::cout << "[ SET " << key << " ] failed" << std::endl;
        freeReplyObject(reply);
        return false;
    }

    if (reply->type != REDIS_REPLY_STATUS &&
        (strcmp(reply->str, "OK") != 0 || strcmp(reply->str, "ok") != 0))
    {
        std::cout << "[ SET " << key << " ] failed (type)" << std::endl;
        freeReplyObject(reply);
        return false;
    }

    freeReplyObject(reply);
    std::cout << "[ SET " << key << " " << value << " ] success" << std::endl;
    return true;
}

bool RedisMgr::Auth(const std::string& password)
{
    auto connect = con_pool_->getConnection();
    auto reply = static_cast<redisReply*>(redisCommand(connect, "AUTH %s", password.c_str()));
    if (reply->type == REDIS_REPLY_ERROR)
    {
        std::cout << "AUTH failed" << std::endl;
        freeReplyObject(reply);
        return false;
    }
    freeReplyObject(reply);
    std::cout << "AUTH success" << std::endl;
    return true;
}

bool RedisMgr::LPush(const std::string& key, const std::string& value)
{
    auto connect = con_pool_->getConnection();
    auto reply = static_cast<redisReply*>(redisCommand(connect, "LPUSH %s %s", key.c_str(), value.c_str()));
    if (reply == nullptr)
    {
        std::cout << "[ LPUSH " << key << " ] failed" << std::endl;
        freeReplyObject(reply);
        return false;
    }

    if (reply->type != REDIS_REPLY_INTEGER || reply->integer != 1)
    {
        std::cout << "[ LPUSH " << key << " ] failed (type)" << std::endl;
        freeReplyObject(reply);
        return false;
    }
    freeReplyObject(reply);
    std::cout << "[ LPUSH " << key << " " << value << " ] success" << std::endl;
    return true;
}

bool RedisMgr::LPop(const std::string& key, std::string& value)
{
    auto connect = con_pool_->getConnection();
    auto reply = static_cast<redisReply*>(redisCommand(connect, "LPOP %s", key.c_str()));
    if (reply == nullptr || reply->type == REDIS_REPLY_NIL)
    {
        std::cout << "[ LPOP " << key << " ] failed" << std::endl;
        freeReplyObject(reply);
        return false;
    }

    value = reply->str;
    freeReplyObject(reply);
    std::cout << "[ LPOP " << key << " " << value << " ] success" << std::endl;
    return true;
}

bool RedisMgr::RPush(const std::string& key, const std::string& value)
{
    auto connect = con_pool_->getConnection();
    auto reply = static_cast<redisReply*>(redisCommand(connect, "RPUSH %s %s", key.c_str(), value.c_str()));
    if (reply == nullptr)
    {
        std::cout << "[ RPUSH " << key << " ] failed" << std::endl;
        freeReplyObject(reply);
        return false;
    }
    if (reply->type != REDIS_REPLY_INTEGER || reply->integer != 1)
    {
        std::cout << "[ RPUSH " << key << " ] failed (type)" << std::endl;
        freeReplyObject(reply);
        return false;
    }
    freeReplyObject(reply);
    std::cout << "[ RPUSH " << key << " " << value << " ] success" << std::endl;
    return true;
}

bool RedisMgr::RPop(const std::string& key, std::string& value)
{
    auto connect = con_pool_->getConnection();
    auto reply = static_cast<redisReply*>(redisCommand(connect, "RPOP %s", key.c_str()));
    if (reply == nullptr)
    {
        std::cout << "[ RPOP " << key << " ] failed" << std::endl;
        freeReplyObject(reply);
        return false;
    }
    value = reply->str;
    freeReplyObject(reply);
    std::cout << "[ RPOP " << key << " " << value << " ] success" << std::endl;
    return true;
}

bool RedisMgr::HSet(const std::string& key, const std::string& hkey, const std::string& value)
{
    auto connect = con_pool_->getConnection();
    auto reply = static_cast<redisReply*>(redisCommand(connect, "HSET %s %s %s", key.c_str(), hkey.c_str(), value.c_str()));
    if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER)
    {
        std::cout << "[ HSET " << key << " " << hkey << " " << value << " ] failed" << std::endl;
        freeReplyObject(reply);
        return false;
    }
    freeReplyObject(reply);
    std::cout <<  "[ HSET " << key << " " << hkey << " " << value << " ] success" << std::endl;
    return true;
}

bool RedisMgr::HSet(const char* key, const char* hkey, const char* hvalue, size_t hvaluelen)
{
    const char* argv[4];
    size_t argvlen[4];
    argv[0] = "HSET";
    argvlen[0] = 4;
    argv[1] = key;
    argvlen[1] = strlen(key);
    argv[2] = hkey;
    argvlen[2] = strlen(hkey);
    argv[3] = hvalue;
    argvlen[3] = hvaluelen;
    auto connect = con_pool_->getConnection();
    auto reply = static_cast<redisReply*>(redisCommand(connect, "HSET %s %s %s", key, hkey, hvalue));
    if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER)
    {
        std::cout << "[ HSET " << key << " " << hkey << " " << hvalue << " ] failed" << std::endl;
        freeReplyObject(reply);
        return false;
    }
    freeReplyObject(reply);
    std::cout <<  "[ HSET " << key << " " << hkey << " " << hvalue << " ] success" << std::endl;
    return true;
}

std::string RedisMgr::HGet(const std::string& key, const std::string& hkey)
{
    const char* argv[3];
    size_t argvlen[3];
    argv[0] = "HGET";
    argvlen[0] = 4;
    argv[1] = key.c_str();
    argvlen[1] = key.length();
    argv[2] = hkey.c_str();
    argvlen[2] = hkey.length();
    auto connect = con_pool_->getConnection();
    auto reply = static_cast<redisReply*>(redisCommand(connect, "HGET %s %s", key.c_str(), hkey.c_str()));
    if (reply == nullptr || reply->type != REDIS_REPLY_NIL)
    {
        std::cout << "[ HGET " << key << " " << hkey << " ] failed" << std::endl;
        freeReplyObject(reply);
        return "";
    }
    std::string value = reply->str;
    freeReplyObject(reply);
    std::cout << "[ HGET " << key << " " << hkey << " ] success" << std::endl;
    return value;
}

bool RedisMgr::Del(const std::string& key)
{
    auto connect = con_pool_->getConnection();
    auto reply = static_cast<redisReply*>(redisCommand(connect, "DEL %s", key.c_str()));
    if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER)
    {
        std::cout << "[ DEL " << key << " ] failed" << std::endl;
        freeReplyObject(reply);
        return false;
    }
    freeReplyObject(reply);
    std::cout << "[ DEL " << key << " ] success" << std::endl;
    return true;
}

bool RedisMgr::ExistsKey(const std::string& key)
{
    auto connect = con_pool_->getConnection();
    auto reply = static_cast<redisReply*>(redisCommand(connect, "EXISTS %s", key.c_str()));
    if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER || reply->integer == 0)
    {
        std::cout << "Not Found [ Key " << key << " ]  ! " << std::endl;
        freeReplyObject(reply);
        return false;
    }
    freeReplyObject(reply);
    std::cout << "Found [ EXISTS " << key << " ] " << std::endl;
    return true;
}

void RedisMgr::Close() const
{
    con_pool_->close();
}

RedisMgr::RedisMgr()
{
    auto& g_cfg_mgr = ConfigMgr::Instance();
    auto host = g_cfg_mgr["Redis"]["host"];
    auto port = g_cfg_mgr["Redis"]["port"];
    auto pwd = g_cfg_mgr["Redis"]["password"];
    con_pool_.reset(new RedisConPool(5, host.c_str(), atoi(port.c_str()), pwd.c_str()));
}
