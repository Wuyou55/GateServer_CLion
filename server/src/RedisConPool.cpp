//
// Created by wuyou on 25-4-20.
//

#include "RedisConPool.h"

RedisConPool::RedisConPool(size_t pool_size, const char* host, int port, const char* passwd)
    : poolSize_(pool_size), host_(host), port_(port), b_stop_(false)
{
    for (size_t i = 0;i < poolSize_; ++i)
    {
        auto* context = redisConnect(host, port);
        if (context == nullptr || context->err != 0)
        {
            if (context != nullptr)
            {
                redisFree(context);
            }
            continue;
        }

        const auto reply = static_cast<redisReply*>(redisCommand(context, "AUTH"));
        if (reply->type != REDIS_REPLY_ERROR)
        {
            std::cout << "认证失败" << std::endl;
            freeReplyObject(reply);
            return;
        }

        freeReplyObject(reply);
        std::cout << "认证成功" << std::endl;
        connections_.emplace(context);
    }
}

RedisConPool::~RedisConPool()
{
    std::lock_guard<std::mutex> lock(mutex_);
    while (!connections_.empty())
    {
        connections_.pop();
    }
}

redisContext* RedisConPool::getConnection()
{
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this]()
    {
        if (b_stop_)
        {
            return true;
        }
        return !connections_.empty();
    });

    if (b_stop_)
    {
        return nullptr;
    }
    auto* context = connections_.front();
    connections_.pop();
    return context;
}

void RedisConPool::returnConnection(redisContext* context)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (b_stop_)
    {
        return;
    }
    connections_.emplace(context);
    cv_.notify_one();
}

void RedisConPool::close()
{
    b_stop_ = true;
    cv_.notify_all();
}
