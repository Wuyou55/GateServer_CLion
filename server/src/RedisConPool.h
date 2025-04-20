//
// Created by wuyou on 25-4-20.
//

#ifndef REDISCONPOOL_H
#define REDISCONPOOL_H

#include "Const.h"
#include <hiredis/hiredis.h>

class RedisConPool {
public:
    RedisConPool(size_t pool_size, const char* host, int port, const char* passwd);
    ~RedisConPool();
    redisContext* getConnection();
    void returnConnection(redisContext* context);
    void close();

private:
    std::atomic<bool> b_stop_;
    size_t poolSize_;
    const char* host_;
    int port_;
    std::queue<redisContext*> connections_;
    std::mutex mutex_;
    std::condition_variable cv_;
};



#endif //REDISCONPOOL_H
