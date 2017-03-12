//
// Created by justin on 2/26/17.
//

#pragma once

#include <functional>

namespace DURIANVER {

#define LISTEN_QUEUE_LENGTH 1024
#define BUFFER_SIZE 1024
#define MAXENVETS 1024

    class Epoll {
    public:
        explicit Epoll(int _port);
        ~Epoll();
        Epoll(const Epoll&)=delete;
        Epoll& operator=(const Epoll&)=delete;

        int start();
        typedef std::function<void(int)> taskcallbackfunc;
        void setTaskCallback(taskcallbackfunc taskFunc);

    private:
        int makeSocketNonblocking(int fd);
        int getListenFd(int port);
        int port;
        int listenFd;
        int epollFd;
        taskcallbackfunc taskCallbackFunc;
    };
}
