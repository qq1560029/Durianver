//
// Created by justin on 3/19/17.
//

#pragma once

namespace DURIANVER
{

class Loop;

class TcpServer
{
  public:
    TcpServer(int threadNum, int port);
    ~TcpServer();

  private:
    int threadNum_;
    int port_;
};
}