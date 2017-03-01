//
// Created by justin on 3/1/17.
//

#include "threadpoll.h"

namespace DURIANVER {
    threadpoll::threadpoll(int _threadNum) : threadNum(_threadNum) {
        std::vector<std::thread> tpoll(threadNum);
        for(int i=0;i<threadNum;i++){
            //tpoll[i]=
        }
    }
}