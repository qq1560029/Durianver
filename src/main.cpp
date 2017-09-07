#include <iostream>
#include "unistd.h"
#include "logwrap.h"
#include "acceptor.h"
#include "loop.h"
#include "epollevent.h"
#include "tcpserver.h"

using namespace DURIANVER;

int main()
{
    if (0 != daemon(1,0))
    {
        std::cout << "daemon failed" << std::endl;
        exit(1);
    }
    //auto my_logger = spdlog::daily_logger_mt(LOGNAME, "logs/daily", 2, 30);
    auto my_logger = spdlog::basic_logger_mt(LOGNAME, "logs/daily");
    my_logger->set_level(spdlog::level::debug);
    my_logger->flush_on(spdlog::level::debug);
    LOGINFO("*********server begin*********");

    TcpServer server(4, 18868);

    LOGINFO("*********server exit*********");
    return 0;
}