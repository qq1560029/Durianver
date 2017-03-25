//
// Created by justin on 3/22/17.
//

#include "acceptor.h"
#include "unistd.h"
#include "netinet/in.h"
#include "memory.h"
#include "socketwrap.h"
#include "logging.h"

#define LISTEN_QUEUE_LENGTH 1024

namespace DURIANVER {

    Acceptor::Acceptor(int port,EpollEvent& epoll):port_(port),epoll_(epoll) {
        listenFd_=getListenFd(port_);
        if(listenFd_<0){
            LOGERR<<"Creat listen fd failed";
            exit(0);
        }
        SocketWrap accept(listenFd_);
        accept.setReadCallBack(std::bind(&Acceptor::readCb,this));
        accept.enableRead();
    }

    void Acceptor::readCb() {
        struct sockaddr inAddr;
        socklen_t inAddrLen;
        int inFd = accept(listenFd_, &inAddr, &inAddrLen);
        if(inFd>0){

        }
        else{
            LOGERR<<"Acceptor readcallback accept fialed";
            exit(0);
        }
    }

    int Acceptor::getListenFd(int port){
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

}