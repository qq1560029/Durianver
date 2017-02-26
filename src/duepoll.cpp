#include "duepoll.h"

//
// Created by justin on 2/26/17.
//
namespace EPOLL {

    Epoll::Epoll(int _port) : port(_port) {

    }

    Epoll::~Epoll() {

    }

    int Epoll::init(){
        listenFd=getListenFd(port);
        if(listenFd<0)
            throw "Get listen fd failed";

        if(makeSocketNonblocking(listenFd)<0)
            throw "Make listen socket non-blocking faild";

        struct epoll_event event;
        epollFd=epoll_create(1);
        if(epollFd<0)
            throw "Create epoll failed";
        event.data.fd=listenFd;
        event.events=EPOLLIN|EPOLLET;
        int state=epoll_ctl(epollFd,EPOLL_CTL_ADD,listenFd,&event);
        if(state<0)
            throw "Epoll add listenFd failed";

        std::unique_ptr<epoll_event[]> events(new epoll_event[MAXENVETS]);

        while(1){
            
        }

        close(epollFd);
        close(listenFd);
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
        bzero(&serverAddr, sizeof(serverAddr));
        serverAddr.sin_family=AF_INET;
        serverAddr.sin_addr.s_addr=htons(INADDR_ANY);
        serverAddr.sin_port=htons(port);

        int listenFd=socket(PF_INET,SOCK_STREAM,0);  //AF_INET PF_INET almost same
        if(listenFd<0)
            return -1;

        int flag=1;
        if(setsockopt(listenFd,SOL_SOCKET,SO_REUSEADDR,(const void *)&flag, sizeof(int))<0)
            return -1;

        if(bind(listenFd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)))
            return -1;

        if(listen(listenFd,LISTEN_QUEUE_LENGTH)<0)
            return -1;

        return listenFd;
    }
}

