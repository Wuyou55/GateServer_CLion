//
// Created by wuyou on 25-4-16.
//

#ifndef VARIFYGRPCCLIENT_H
#define VARIFYGRPCCLIENT_H
#include "Const.h"
#include <grpcpp/grpcpp.h>
#include <myproto/message.grpc.pb.h>
#include "Singleton.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::VerifyService;
using message::GetVerifyReq;
using message::GetVerifyRsp;

class RPConPool
{
public:
    RPConPool(std::size_t pool_size, std::string host, std::string port);
    ~RPConPool();

    void Close();
    std::unique_ptr<VerifyService::Stub> GetConnection();
    void reConnection(std::unique_ptr<VerifyService::Stub> context);
private:
    std::atomic<bool> b_stop_;
    std::size_t pool_size_;
    std::string host_;
    std::string port_;
    std::queue<std::unique_ptr<VerifyService::Stub>> connections_;
    std::condition_variable cv_;
    std::mutex mutex_;
};

class VerifyGrpcClient : public Singleton<VerifyGrpcClient>
{
    friend class Singleton<VerifyGrpcClient>;

public:
    GetVerifyRsp GetVerifyCode(std::string email) const;
private:
    VerifyGrpcClient();
    std::unique_ptr<RPConPool> pool_;
};


#endif //VARIFYGRPCCLIENT_H
