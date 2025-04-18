//
// Created by wuyou on 25-4-19.
//

#include "AsioIOServicePool.h"

AsioIOServicePool::AsioIOServicePool(std::size_t size_)
    : ioServices_(size_), works_(size_), next_IOService_(0)
{
    for (std::size_t i = 0; i < size_; ++i)
    {
        works_[i] = std::make_unique<Work>(ioServices_[i].get_executor());
    }
    // 遍历多个ioservice，创建多个线程，每个线程内部启动ioservice
    for (auto& ioService : ioServices_)
    {
        threads_.emplace_back([this, &ioService]()
        {
            ioService.run();
        });
    }
}

AsioIOServicePool::~AsioIOServicePool()
{
    Stop();
    std::cout << "AsioIOServicePool destruct" << std::endl;
}

boost::asio::io_context& AsioIOServicePool::GetIOService()
{
    const std::size_t index = next_IOService_++;
    if (next_IOService_ == ioServices_.size())
    {
        next_IOService_ = 0;
    }
    return ioServices_[index];
}

void AsioIOServicePool::Stop()
{
    // 先取消所有 work_guard，让 io_context 能自动退出 run()
    for (const auto& guard : works_)
    {
        guard->reset(); // 新写法，释放 keep-alive
    }

    // 确保 io_context 自己也 stop 掉（防止有其他阻塞任务）
    for (auto& io : ioServices_)
    {
        io.stop();
    }
    for (auto& thread : threads_)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}
