//
// Created by justin on 3/19/17.
//

#include <unistd.h>
#include "socketwrap.h"
namespace DURIANVER {

    SocketWrap::SocketWrap(int fd):socket_(fd) {

    }

    SocketWrap::~SocketWrap() {
        close(socket_);
    }

    void SocketWrap::handleEvent() {
        if(event_.events&&(EPOLLIN)){
            handleRead();
        }
    }

}