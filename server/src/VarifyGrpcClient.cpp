//
// Created by wuyou on 25-4-16.
//

#include "VarifyGrpcClient.h"

GetVarifyRsp VarifyGrpcClient::GetVarifyCode(std::string email) const
{
    ClientContext context;
    GetVarifyRsp reply;
    GetVarifyReq request;
    request.set_email(email);

    if (const Status status = stub_->GetVarifyCode(&context, request, &reply); status.ok())
    {
        return reply;
    }
    reply.set_error(ErrorCode::RPCFailed);
    return reply;
}

VarifyGrpcClient::VarifyGrpcClient()
{
    const std::shared_ptr<Channel> channel = grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials());
    stub_ = VarifyService::NewStub(channel);
}
