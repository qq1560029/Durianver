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
            int acWrapsNum=epoll_->epoll(activeWraps);
            for (int i = 0; i <acWrapsNum ; ++i) {
                activeWraps[i]->handleEvent();
            }

        }
        return 0;
    }
}
