//
// Created by justin on 3/19/17.
//

#pragma once

#include "epollevent.h"
#include "socketwrap.h"
#include <vector>

namespace DURIANVER {
    class Loop {
    public:
        Loop(EpollEvent* epoll);

        ~Loop();

        int init();

        int startLoop();

    private:
        EpollEvent* epoll_;
        int looping_;
        std::vector<SocketWrap*> activeWraps;
    };
}

