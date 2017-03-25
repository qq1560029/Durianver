//
// Created by justin on 3/19/17.
//

#include "unistd.h"
#include "fcntl.h"
#include "socketwrap.h"
#include "logging.h"

namespace DURIANVER {

    SocketWrap::SocketWrap(int fd):socket_(fd) {
        if(makeSocketNonblocking()<0){
            LOGERR<<"Make socket non blocking failed";
            exit(0);
        }
    }

    SocketWrap::~SocketWrap() {
        close(socket_);
    }

    void SocketWrap::handleEvent() {
        if ((recvEvent_ & EPOLLHUP) && !(recvEvent_ & EPOLLIN))
        {
            if (closeCallBack_)
                closeCallBack_();
        }

        if (recvEvent_ & EPOLLERR)
        {
            if (errorCallBack_)
                errorCallBack_();
        }
        if (recvEvent_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))  //EPOLLRDHUP:opposite close the connection
        {
            if (readCallBack_)
                readCallBack_();
        }
        if (recvEvent_ & EPOLLOUT)
        {
            if (writeCallBack_)
                writeCallBack_();
        }
    }

    int SocketWrap::makeSocketNonblocking(){
        int flags,state;

        flags=fcntl(socket_,F_GETFL,0);
        if(flags==-1){
            return -1;
        }
        flags |=O_NONBLOCK;

        state=fcntl(socket_,F_SETFL,flags);
        if(state==-1){
            return -1;
        }
        return 0;
    }

    void SocketWrap::updateWrap() {

    }

}