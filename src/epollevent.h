//
// Created by justin on 3/19/17.
//

#pragma once

#include <sys/epoll.h>
#include "socketwrap.h"
#include <vector>

namespace DURIANVER {
    class EpollEvent {
    public:
        EpollEvent();

        ~EpollEvent();

        int epoll(std::vector<SocketWrap *> activeWraps);
        int ctl(unsigned int epollCtl, SocketWrap *wrap);

    private:
        int epollFd_;
        int maxEvents_;
        std::vector<epoll_event> events_;
    };
}

