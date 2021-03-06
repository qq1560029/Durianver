//
// Created by justin on 3/19/17.
//

#pragma once

#include <functional>
#include "sys/epoll.h"

namespace DURIANVER
{

class Loop;

class SocketWrap
{
  public:
    SocketWrap(Loop *loop, const int &loopId, const int &fd);
    ~SocketWrap();

    void handleEvent();

    void setReadCallBack(const std::function<void(void)> &cb) { readCallBack_ = cb; }
    void setWriteCallBack(const std::function<void(void)> &cb) { writeCallBack_ = cb; }

    void handleRead() { readCallBack_(); }
    void handleWrite() { writeCallBack_(); }

    void enableRead()
    {
        event_ |= readEvent_;
        updateWrap();
    }
    void disableRead()
    {
        event_ &= ~readEvent_;
        updateWrap();
    }
    void enableWrite()
    {
        event_ |= writeEvent_;
        updateWrap();
    }
    void disableWrite()
    {
        event_ &= ~writeEvent_;
        updateWrap();
    }
    void disableAll()
    {
        event_ = noneEvent_;
        updateWrap();
    }
    int isReadEnabled() const { return event_ & readEvent_; }
    int isWriteEnabled() const { return event_ & writeEvent_; }

    void setEvent(int event) { recvEvent_ = event; }
    int getEvent() const { return event_; }
    int getSocketFd() const { return socket_; }
    int makeSocketNonblocking();

    void addToEpoll(bool b) { isAddToEpoll_ = b; }
    bool isAddToEpoll() const { return isAddToEpoll_; }
    int getLoopId() const { return loopId_; }

  private:
    bool isAddToEpoll_;
    Loop *loop_;
    const int loopId_;
    int socket_;
    int event_;
    int recvEvent_;
    std::function<void(void)> readCallBack_;
    std::function<void(void)> writeCallBack_;
    std::function<void(void)> closeCallBack_;
    std::function<void(void)> errorCallBack_;

    const int noneEvent_ = 0;
    const int readEvent_ = EPOLLIN | EPOLLPRI;
    const int writeEvent_ = EPOLLOUT;
    void updateWrap();
};
}
