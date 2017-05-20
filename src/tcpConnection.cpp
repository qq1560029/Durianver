//
// Created by justin on 3/22/17.
//

#include "unistd.h"
#include "tcpConnection.h"

namespace DURIANVER {

    TcpConnection::TcpConnection(Loop *loop, int inFd):loop_(loop) {
        conSocketWrap_=new SocketWrap(loop_,inFd);
        conSocketWrap_->setReadCallBack(std::bind(&TcpConnection::socketReadCb,this));
        conSocketWrap_->setWriteCallBack(std::bind(&TcpConnection::socketWriteCb,this));
        conSocketWrap_->enableRead();
    }

    TcpConnection::~TcpConnection() {
        delete conSocketWrap_;
    }

    void TcpConnection::socketReadCb() {
        char buf[512];
        int n=read(conSocketWrap_->getSocketFd(),buf,sizeof(buf));
        if(n==0)
            delete this;
        write(conSocketWrap_->getSocketFd(),buf,n);
        if(readOkCallBack_)
            readOkCallBack_;
    }

    void TcpConnection::socketWriteCb() {
        if(writeOkCallBack_)
            writeOkCallBack_;
    }
}
