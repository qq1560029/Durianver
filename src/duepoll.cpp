#include "duepoll.h"

#include <vector>
#include <memory>

#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <memory.h>
#include <sys/epoll.h>

#include "threadpoll.h"
#include "logging.h"
//
// Created by justin on 2/26/17.
//
namespace DURIANVER {
    Epoll::Epoll(int _port) : port(_port) {

    }

    Epoll::~Epoll() {
        if(epollFd>=0){
            close(epollFd);
        }
        if(listenFd>=0) {
            close(listenFd);
        }
    }

    int Epoll::start(){
        listenFd=getListenFd(port);
        if(listenFd<0){
            logERROR<<"Get listen fd failed";
            return -1;
        }

        if(makeSocketNonblocking(listenFd)<0) {
            logERROR << "Make listen socket non-blocking faild";
            return -1;
        }

        struct epoll_event event;
        epollFd=epoll_create(1);
        if(epollFd<0) {
            logERROR<<"Create epoll failed";
            return -1;
        }
        event.data.fd=listenFd;
        event.events=EPOLLIN|EPOLLET;
        if(epoll_ctl(epollFd,EPOLL_CTL_ADD,listenFd,&event)<0) {
            logERROR<< "Epoll add listenFd failed";
            return -1;
        }
        std::vector<epoll_event> events(MAXENVETS);
        ThreadPoll threadPoll(std::thread::hardware_concurrency()-1);  //left one thread for epoll

        while(1){
            int nums=epoll_wait(epollFd,events.data(),MAXENVETS,-1);
            for (int i = 0; i < nums ; ++i) {
                if(listenFd==events[i].data.fd){
                    while(1) {
                        struct sockaddr inAddr;
                        socklen_t inAddrLen;
                        int inFd = accept(listenFd, &inAddr, &inAddrLen);
                        if (inFd < 0) {
                            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                                break;
                            } else {
                                logWARN<<"Accept new connect failed";
                            }
                        }
                        if (makeSocketNonblocking(inFd) < 0) {
                            logWARN<<"Make new connect fd non-blocking failed";
                        }
                        event.data.fd = inFd;
                        event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
                        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, inFd, &event) < 0) {
                            logWARN<<"Add new connect fd to epoll failed";
                        }
                        continue;
                    }
                }
                else if((events[i].events& EPOLLERR)||(events[i].events& EPOLLHUP)||(!(events[i].events& EPOLLIN))){
                    //print epoll error
                    close(events[i].data.fd);
                    continue;
                }
                else{
                    int fd=events[i].data.fd;
                    threadPoll.submit<std::function<void()>>(bind(taskCallbackFunc,fd));
                }
            }
        }
        return 0;
    }

    int Epoll::makeSocketNonblocking(int fd){
        int flags,state;

        flags=fcntl(fd,F_GETFL,0);
        if(flags==-1){
            return -1;
        }
        flags |=O_NONBLOCK;

        state=fcntl(fd,F_SETFL,flags);
        if(state==-1){
            return -1;
        }
        return 0;
    }

    int Epoll::getListenFd(int port){
        struct sockaddr_in serverAddr;
        bzero((char *)&serverAddr, sizeof(serverAddr));
        serverAddr.sin_family=AF_INET;
        serverAddr.sin_addr.s_addr=htonl(INADDR_ANY);
        serverAddr.sin_port=htons((unsigned short)port);

        int listenFd=socket(AF_INET,SOCK_STREAM,0);  //AF_INET PF_INET almost same
        if(listenFd<0)
            return -1;

        int flag=1;
        if(setsockopt(listenFd,SOL_SOCKET,SO_REUSEADDR,(const void *)&flag, sizeof(int))<0)
            return -1;

        if(bind(listenFd,(struct sockaddr*)&serverAddr,sizeof(serverAddr))<0)
            return -1;

        if(listen(listenFd,LISTEN_QUEUE_LENGTH)<0)
            return -1;

        return listenFd;
    }

    void Epoll::setTaskCallback(taskcallbackfunc taskFunc) {
        taskCallbackFunc=taskFunc;
    }
}

