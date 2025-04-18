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

class VerifyGrpcClient : public Singleton<VerifyGrpcClient>
{
    friend class Singleton<VerifyGrpcClient>;

public:
    GetVerifyRsp GetVerifyCode(std::string email) const;
private:
    VerifyGrpcClient();
    std::unique_ptr<VerifyService::Stub> stub_;
};


#endif //VARIFYGRPCCLIENT_H
