//
// Created by justin on 3/19/17.
//

#pragma once

#include <vector>
#include "epollevent.h"
#include "socketwrap.h"

namespace DURIANVER {
    class Loop {
    public:
        Loop(EpollEvent* epoll);
       // ~Loop();

       // int init();
        int startLoop();
        void updateSocketWraps(SocketWrap* sw);

    private:
        EpollEvent* epoll_;
        int looping_;
        std::vector<SocketWrap*> activeWraps_;

    };
}

