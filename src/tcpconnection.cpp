//
// Created by justin on 3/22/17.
//

#include <string>
#include <thread>
#include "unistd.h"
#include "tcpconnection.h"
#include "socketwrap.h"
#include "logwrap.h"
#include "loop.h"
#include "buffer.h"

#define EXTBUFLEN 32768

namespace DURIANVER
{

thread_local char extraBuf[EXTBUFLEN];

TcpConnection::TcpConnection(Loop *loop, const int &loopId, const int &inFd) : loop_(loop), loopId_(loopId)
{
    conSocketWrap_ = new SocketWrap(loop_, loopId_, inFd);
    conSocketWrap_->setReadCallBack(std::bind(&TcpConnection::socketReadCb, this));
    conSocketWrap_->setWriteCallBack(std::bind(&TcpConnection::socketWriteCb, this));
    conSocketWrap_->enableRead();
    inputBuf_ = new Buffer();
    outputBuf_ = new Buffer();
    iov_[1].iov_base = extraBuf;
    iov_[1].iov_len = EXTBUFLEN;
}

TcpConnection::~TcpConnection()
{
    delete conSocketWrap_;
    delete inputBuf_;
    delete outputBuf_;
}

void TcpConnection::socketReadCb()
{
    iov_[0].iov_base = inputBuf_->wPtr();
    int writeLen = inputBuf_->writable();
    iov_[0].iov_len = writeLen;
    int n = readv(conSocketWrap_->getSocketFd(), iov_, 2);
    if (n == 0)
    {
        shutDown();
    }
    else if (n < 0)
    {
        if (errno == EAGAIN)
        {
            return;
        }
        LOGERR("read socket:{} err", conSocketWrap_->getSocketFd());
        shutDown();
    }
    else
    {
        if (n > writeLen)
        {
            inputBuf_->append(extraBuf, n - writeLen);
        }
        std::string echo=inputBuf_->retriveAsString();
        LOGDEBUG("echo:{}", echo);
        write(conSocketWrap_->getSocketFd(), echo.c_str(), echo.size());
        if (readOkCallBack_)
            readOkCallBack_;
    }
}

void TcpConnection::socketWriteCb()
{
    if (writeOkCallBack_)
        writeOkCallBack_;
}

void TcpConnection::shutDown()
{
    //TODO clear write buffer
    loop_->delCon(this);
}
}
