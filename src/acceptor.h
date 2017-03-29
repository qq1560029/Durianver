//
// Created by justin on 3/22/17.
// Responsible for accept listenFd and create tcp connection.
//

#pragma once

#include "loop.h"

namespace DURIANVER {
    class Acceptor {
    public:
        Acceptor(int port,Loop* loop);
        ~Acceptor();

        void setAcceptCallBack(const std::function<void(int)>& cb){acceptCallBack_=cb;}

    private:
        Loop* loop_;
        int port_;
        int listenFd_;
        SocketWrap* accept_;
        std::function<void(int)> acceptCallBack_;
        int getListenFd(int port);
        void readCallBack();
    };
}

