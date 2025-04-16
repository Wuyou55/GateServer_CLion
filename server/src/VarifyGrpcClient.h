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

using message::VarifyService;
using message::GetVarifyReq;
using message::GetVarifyRsp;

class VarifyGrpcClient : public Singleton<VarifyGrpcClient>
{
    friend class Singleton<VarifyGrpcClient>;

public:
    GetVarifyRsp GetVarifyCode(std::string email) const;
private:
    VarifyGrpcClient();
    std::unique_ptr<VarifyService::Stub> stub_;
};


#endif //VARIFYGRPCCLIENT_H
