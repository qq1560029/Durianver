//
// Created by justin on 2/26/17.
//

#ifndef DURIANVER_EPOLL_H
#define DURIANVER_EPOLL_H
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <memory>
#include <cstdlib>
#include <sys/epoll.h>

namespace EPOLL {

#define LISTEN_QUEUE_LENGTH 1024
#define BUFFER_SIZE 1024
#define MAXENVETS 1024

    class Epoll {
    public:
        explicit Epoll(int _port);
        ~Epoll();
        int init();

    private:
        int makeSocketNonblocking(int fd);
        int getListenFd(int port);
        int port;
        int listenFd;
        int epollFd;
    };
}


#endif //DURIANVER_EPOLL_H
