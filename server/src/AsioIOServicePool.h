//
// Created by wuyou on 25-4-19.
//

#ifndef ASIOIOSERVICEPOOL_H
#define ASIOIOSERVICEPOOL_H
#include "Const.h"
#include "Singleton.h"
#include <boost/asio.hpp>

class AsioIOServicePool : public Singleton<AsioIOServicePool>
{
    friend Singleton<AsioIOServicePool>;

public:
    using IOService = boost::asio::io_context;
    using Work = boost::asio::executor_work_guard<IOService::executor_type>;
    using WorkPtr = std::unique_ptr<Work>;

    ~AsioIOServicePool();

    AsioIOServicePool(const AsioIOServicePool&) = delete;
    AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;

    boost::asio::io_context& GetIOService();
    void Stop();

private:
    explicit AsioIOServicePool(std::size_t size_ = 2);
    std::vector<IOService> ioServices_;
    std::vector<WorkPtr> works_;
    std::vector<std::thread> threads_;
    std::size_t next_IOService_;
};


#endif //ASIOIOSERVICEPOOL_H
