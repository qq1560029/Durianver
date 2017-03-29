#include <iostream>
#include "logging.h"
#include "acceptor.h"
#include "loop.h"
#include "epollevent.h"

using namespace DURIANVER;
using namespace std::placeholders;

void acceptIn(int inFd){
    char buf[512];
    int n=read(inFd,buf,sizeof(buf));
    write(inFd,buf,n);
    close(inFd);
}

int main() {
    LOGINFO<<"Init server, port:18868";
    EpollEvent epollevent;
    Loop loop(&epollevent);
    Acceptor accept(18868,&loop);
    accept.setAcceptCallBack(std::bind(acceptIn,_1));
    loop.startLoop();
    return 0;
}