//
// Created by justin on 3/19/17.
//

#include "loop.h"
#include "epollevent.h"

namespace DURIANVER{

    Loop::Loop(EpollEvent *epoll):epoll_(epoll),looping_(true) {

    }

    int Loop::startLoop() {
        while(looping_){
            int acWrapsNum=epoll_->epoll(activeWraps_);
            for (int i = 0; i <acWrapsNum ; ++i) {
                activeWraps_[i]->handleEvent();
            }

        }
        return 0;
    }

    void Loop::updateSocketWraps(SocketWrap *sw) {
        if (sw->isAddToEpoll()== false) {
            epoll_->ctl(EPOLL_CTL_ADD,sw);
        }
        else{
            epoll_->ctl(EPOLL_CTL_MOD,sw);
        }
    }
}
