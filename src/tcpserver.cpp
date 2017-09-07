//
// Created by justin on 3/19/17.
//

#include "tcpserver.h"
#include "logwrap.h"
#include "loop.h"
#include "acceptor.h"

namespace DURIANVER
{

TcpServer::TcpServer(int threadNum, int port) : threadNum_(threadNum), port_(port)
{
    Loop loop(threadNum_);
    Acceptor accept(port_, &loop);
    LOGINFO("TcpServer Start, port:{} threadNum:{}", port_, threadNum_);
    loop.startLoop();
}

TcpServer::~TcpServer()
{

}

}