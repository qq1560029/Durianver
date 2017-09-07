//
// Created by justin on 3/22/17.
// Resposible for read/write and close socket.
//

#pragma once

#include <functional>
#include "sys/uio.h"

namespace DURIANVER
{
  
class SocketWrap;
class Loop;
class Buffer;

//enum{TCPCONNECTED,TCPWRITING,TCPREADING};
class TcpConnection
{
public:
  TcpConnection(Loop *loop, const int &loopId, const int &inFd);
  ~TcpConnection();

  void setReadOkCallBack(const std::function<void(int)> &cb) { readOkCallBack_ = cb; }
  void setWriteOkCallBack(const std::function<void(int)> &cb) { writeOkCallBack_ = cb; }
  void shutDown();

private:
  Loop *loop_;
  const int loopId_;
  SocketWrap *conSocketWrap_;
  struct iovec iov_[2];
  Buffer *inputBuf_;
  Buffer *outputBuf_;

  std::function<void(int)> readOkCallBack_;
  std::function<void(int)> writeOkCallBack_;
  void socketWriteCb();
  void socketReadCb();
};
}
