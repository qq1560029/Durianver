//
// Created by justin on 3/19/17.
//

#include "epollevent.h"
#include "logging.h"
#include "socketwrap.h"

namespace DURIANVER {

    EpollEvent::EpollEvent():maxEvents_(512) {
        events_.resize(maxEvents_);
        epollFd_=epoll_create(maxEvents_);
        if(epollFd_<0) {
            LOGERR<<"Create epoll failed";
            exit(0);
        }
    }

    EpollEvent::~EpollEvent() {
        if(epollFd_>0)
            close(epollFd_);
    }

    int EpollEvent::ctl(unsigned int epollCtl, SocketWrap *wrap) {
        struct epoll_event event;
        event.data.ptr=wrap;
        event.events=wrap->getEvent();
        if(epoll_ctl(epollFd_,epollCtl,wrap->getSocketFd(),&event)<0) {
            LOGERR<< "Epoll add listenFd failed";
            return -1;
        }
        wrap->addToEpoll(true);
        return 0;
    }

    int EpollEvent::epoll(std::vector<SocketWrap *> &activeWraps) {
        int nums=epoll_wait(epollFd_,events_.data(),maxEvents_,-1);
        if (nums>activeWraps.size())
            activeWraps.resize(nums);
        for (int i = 0; i < nums; ++i) {
            activeWraps[i]= static_cast<SocketWrap*>(events_[i].data.ptr);
            activeWraps[i]->setEvent(events_[i].events);
        }
        return nums;
    }
}