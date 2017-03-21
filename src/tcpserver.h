//
// Created by justin on 3/19/17.
//

#pragma once

#include "loop.h"

namespace DURIANVER {
    class TcpServer {
    public:
        TcpServer(Loop* loop,int port);
        ~TcpServer();

    private:
        Loop* loop_;
        int port_;
    };
}