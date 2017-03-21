//
// Created by justin on 3/19/17.
//

#pragma once

#include <functional>
#include <sys/epoll.h>

namespace DURIANVER {
    class SocketWrap {
    public:
        SocketWrap(int fd);
        ~SocketWrap();

        void handleEvent();

        void setReadCallBack(std::function<void(int)> &cb){readCallBack_=cb;}
        void setWriteCallBack(std::function<void(int)> &cb){writeCallBack_=cb;}

        void handleRead(){readCallBack_;}
        void handleWrite(){writeCallBack_;}

        void setRead(){event_.events=EPOLLIN|EPOLLONESHOT;}
        void setWrite(){event_.events=EPOLLOUT|EPOLLONESHOT;}

        epoll_event getEvent(){ return event_;}
        int getSocketFd(){ return socket_;}


    private:
        int socket_;
        epoll_event event_;
        std::function<void(int)> readCallBack_;
        std::function<void(int)> writeCallBack_;
    };
}

#endif //DURIANVER_SOCKETWRAP_H
