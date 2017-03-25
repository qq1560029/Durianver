//
// Created by justin on 3/22/17.
// Responsible for accept listenFd and create tcp connection.
//

#pragma once

#include "epollevent.h"

namespace DURIANVER {
    class Acceptor {
    public:
        Acceptor(int port,EpollEvent& epoll);
        ~Acceptor();

    private:
        EpollEvent epoll_;
        int port_;
        int listenFd_;
        int getListenFd(int port);
        void readCb();
    };
}

#endif //DURIANVER_ACCEPTOR_H
