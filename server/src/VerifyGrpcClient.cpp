//
// Created by wuyou on 25-4-16.
//

#include "VerifyGrpcClient.h"
#include "ConfigMgr.h"

RPConPool::RPConPool(std::size_t pool_size, std::string host, std::string port)
    : pool_size_(pool_size), host_(host), port_(port), b_stop_(false)
{
    for (std::size_t i = 0; i < pool_size_; ++i)
    {
        std::shared_ptr<Channel> channel =
            CreateChannel(host_ + ":" + port_, grpc::InsecureChannelCredentials());
        connections_.emplace(VerifyService::NewStub(channel));
    }
}

RPConPool::~RPConPool()
{
    std::lock_guard<std::mutex> lock(mutex_);
    Close();
    while (!connections_.empty())
    {
        connections_.pop();
    }
}

void RPConPool::Close()
{
    b_stop_ = true;
    cv_.notify_all();
}

std::unique_ptr<VerifyService::Stub> RPConPool::GetConnection()
{
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this]()
    {
        if (b_stop_ == true)
        {
            return true;
        }
        return !connections_.empty();
    });

    if (b_stop_ == true)
    {
        return nullptr;
    }

    auto context = std::move(connections_.front());
    connections_.pop();
    return context;
}

void RPConPool::reConnection(std::unique_ptr<VerifyService::Stub> context)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (b_stop_ == true)
    {
        return;
    }
    connections_.emplace(std::move(context));
    cv_.notify_one();
}

GetVerifyRsp VerifyGrpcClient::GetVerifyCode(std::string email) const
{
    ClientContext context;
    GetVerifyRsp reply;
    GetVerifyReq request;
    std::cout << "GetVarifyCode email is " << email << std::endl;
    request.set_email(email);
    auto stub_ = pool_->GetConnection();
    const Status status = stub_->GetVerifyCode(&context, request, &reply);
    std::cout << "status.error_code is " << status.error_code() << ": " << status.error_message() << std::endl;
    if (status.ok())
    {
        pool_->reConnection(std::move(stub_));
        return reply;
    }
    pool_->reConnection(std::move(stub_));
    reply.set_error(ErrorCode::RPCFailed);
    return reply;
}

VerifyGrpcClient::VerifyGrpcClient()
{
    auto& g_cfg_mgr = ConfigMgr::Instance();
    std::string host = g_cfg_mgr["VarifyServer"]["Host"];
    std::string port = g_cfg_mgr["VarifyServer"]["Port"];
    pool_.reset(new RPConPool(5, host, port));
}
