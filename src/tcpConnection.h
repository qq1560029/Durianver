//
// Created by justin on 3/22/17.
// Resposible for read/write and close socket.
//

#pragma once

#include "socketwrap.h"
#include "loop.h"

namespace DURIANVER {
    //enum{TCPCONNECTED,TCPWRITING,TCPREADING};
    class TcpConnection {
    public:
        TcpConnection(Loop *loop, int inFd);
        ~TcpConnection();

        void setReadOkCallBack(const std::function<void(int)>& cb){readOkCallBack_=cb;}
        void setWriteOkCallBack(const std::function<void(int)>& cb){writeOkCallBack_=cb;}

    private:
        Loop* loop_;
        //int state_;
        SocketWrap* conSocketWrap_;

        std::function<void(int)> readOkCallBack_;
        std::function<void(int)> writeOkCallBack_;
        void socketWriteCb();
        void socketReadCb();
    };
}
