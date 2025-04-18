//
// Created by wuyou on 25-4-16.
//

#include "VerifyGrpcClient.h"

GetVerifyRsp VerifyGrpcClient::GetVerifyCode(std::string email) const
{
    ClientContext context;
    GetVerifyRsp reply;
    GetVerifyReq request;
    std::cout << "GetVarifyCode email is " << email << std::endl;
    request.set_email(email);
    const Status status = stub_->GetVerifyCode(&context, request, &reply);
    std::cout << "status.error_code is " << status.error_code() << ": " << status.error_message() << std::endl;
    if (status.ok())
    {
        return reply;
    }
    reply.set_error(ErrorCode::RPCFailed);
    return reply;
}

VerifyGrpcClient::VerifyGrpcClient()
{
    const std::shared_ptr<Channel> channel = grpc::CreateChannel("127.0.0.1:50051",
                                                                 grpc::InsecureChannelCredentials());
    stub_ = VerifyService::NewStub(channel);
}
