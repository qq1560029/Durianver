//
// Created by justin on 3/19/17.
//

#include "loop.h"
#include "epollevent.h"
#include "socketwrap.h"
#include "tcpconnection.h"
#include "logwrap.h"
#include <algorithm>
#include <string>

namespace DURIANVER
{

Loop::Loop(int loopNum) : loopNum_(loopNum), looping_(true)
{
    if (loopNum < 1)
        loopNum_ = 1;
    conNums_.resize(loopNum_, 0);
    activeWraps_.resize(loopNum_);
    for (int i = 0; i < loopNum_; i++){
        EpollEvent *etmp = new EpollEvent();
        epoll_.push_back(etmp);
    }
}

Loop::~Loop()
{
    looping_ = false;
    for (int i = 1; i < loopNum_; i++)
    {
        threads_[i - 1].join();
        delete epoll_[i];
    }
}

void Loop::loopBody(int loopId, EpollEvent* epoll, std::vector<SocketWrap *> &activeWraps)
{
    LOGDEBUG("loopId:{} start", loopId);
    while (looping_)
    {
        int acWrapsNum = epoll->epoll(activeWraps);
        for (int i = 0; i < acWrapsNum; ++i)
        {
            activeWraps[i]->handleEvent();
        }
    }
}

void Loop::delCon(TcpConnection *con)
{
    int loopId = conMap_[con].loopId;
    conNums_[loopId]--;
    conMap_.erase(con);
    delete con;
}

int Loop::startLoop()
{
    if (loopNum_ != 1)
        conNums_[0] = INT32_MAX; //prevent dispatch to main loop when loopNum>1
    //loops to handle connections
    for (int i = 1; i < loopNum_; i++)
    {
        threads_.push_back(std::thread(&Loop::loopBody, this, i, epoll_[i], std::ref(activeWraps_[i])));
    }
    //main loop to dispatch sockets
    LOGDEBUG("start main loop");
    while (looping_)
    {
        int acWrapsNum = epoll_[0]->epoll(activeWraps_[0]);
        if (acWrapsNum < 0)
        {
            return -1;
        }
        for (int i = 0; i < acWrapsNum; ++i)
        {
            activeWraps_[0][i]->handleEvent();
        }
    }
    return 0;
}

void Loop::dispatchSocket(const int &fd)
{
    //dispatch new con to least conNums loop
    const int aimLoopId = std::distance(conNums_.begin(), std::min_element(conNums_.begin(), conNums_.end()));
    TcpConnection *newSocketWrap = new TcpConnection(this, aimLoopId, fd);
    ConInfo conInfo;
    conInfo.loopId = aimLoopId;
    conMap_[newSocketWrap] = conInfo;
    conNums_[aimLoopId]++;
    std::string numstr;
    
    for (int i = 0; i < loopNum_; i++)
    {
        numstr += " " + std::to_string(conNums_[i]);
    }
    LOGDEBUG("new socketfd:{} dispatch to loop:{} loop conNum:{}", fd, aimLoopId, numstr);
}

void Loop::updateSocketWraps(SocketWrap *sw)
{
    const int loopId = sw->getLoopId();
    if (sw->isAddToEpoll() == false)
    {
        epoll_[loopId]->ctl(EPOLL_CTL_ADD, sw);
    }
    else
    {
        epoll_[loopId]->ctl(EPOLL_CTL_MOD, sw);
    }
}
}
