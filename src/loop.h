//
// Created by justin on 3/19/17.
//

#pragma once

#include <vector>
#include <thread>
#include <map>

namespace DURIANVER
{

class EpollEvent;
class SocketWrap;
class TcpConnection;

struct ConInfo{
    int loopId;
};

//LoopId=0: main loop to handle create sockets and dispatch connections to work loops
//LoopId>0: work loops

class Loop
{
  public:
    Loop(int loopNum);
    ~Loop();

    // int init();
    int startLoop();
    void dispatchSocket(const int& fd);
    void updateSocketWraps(SocketWrap *sw);
    void delCon(TcpConnection* con);

  private:
    void loopBody(int loopId, EpollEvent* epoll, std::vector<SocketWrap*> &activeWraps);
    std::vector<EpollEvent*> epoll_;
    std::vector<std::thread> threads_;
    std::map<TcpConnection*,ConInfo> conMap_;
    std::vector<int> conNums_;
    int looping_;
    int loopNum_;
    std::vector<std::vector<SocketWrap *>> activeWraps_;
};
}
