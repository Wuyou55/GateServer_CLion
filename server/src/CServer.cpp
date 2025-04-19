//
// Created by wuyou on 25-4-4.
//

#include "CServer.h"
#include "HttpConnection.h"
#include "AsioIOServicePool.h"

CServer::CServer(boost::asio::io_context& ioc, unsigned short& port)
    : ioc_(ioc), acceptor_(ioc, tcp::endpoint(tcp::v4(), port)), socket_(ioc)
{
}

void CServer::start()
{
    auto self = shared_from_this();
    auto& io_context = AsioIOServicePool::GetInstance()->GetIOService();
    auto new_con = std::make_shared<HttpConnection>(io_context);
    acceptor_.async_accept(new_con->GetSocket(), [self, new_con](const beast::error_code& ec)
    {
        try
        {
            // 出错，放弃这个链接，继续监听其他链接
            if (ec)
            {
                self->start();
                return;
            }

            // 创建新链接，并且创建HttpConnection类的示例管理这个链接
            new_con->Start();

            // 继续监听
            self->start();
        }
        catch (std::exception& exp)
        {
            std::cout << "exception is " << exp.what() << std::endl;
        }
    });
}
