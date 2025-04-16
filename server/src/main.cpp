#include "ConfigMgr.h"
#include "CServer.h"

int main() {
    ConfigMgr g_config_mgr;
    std::string gate_port_str = g_config_mgr["GateServer"]["port"];
    unsigned short gate_port = atoi(gate_port_str.c_str());

    try
    {
        unsigned short port = static_cast<unsigned short>(gate_port);
        net::io_context ioc { 1 };
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](const boost::system::error_code &error, int signal_number)
        {
            if(error)
                return ;

            ioc.stop();
        });
        std::make_shared<CServer>(ioc, port)->start();
        ioc.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return 0;
}