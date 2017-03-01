//
// Created by justin on 3/1/17.
//

#ifndef DURIANVER_THREADPOLL_H
#define DURIANVER_THREADPOLL_H

#include <boost/noncopyable.hpp>
#include <mutex>
#include <thread>
#include <future>
#include <vector>

namespace DURIANVER {

    class threadpoll : boost::noncopyable {
        explicit threadpoll(int _threadNum);

    private:
        int threadNum;
        //std::vector<std::packaged_task>
    };
}

#endif //DURIANVER_THREADPOLL_H
