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
#include "blockingqueue.h"

namespace DURIANVER {
    class ThreadPoll : boost::noncopyable {
    public:
        explicit ThreadPoll(size_t _threadNum):threadNum(_threadNum),done(false){
            unsigned const thread_count=std::thread::hardware_concurrency();
            try
            {
                for(unsigned i=0;i<thread_count;++i)
                {
                    threads.push_back(
                            std::thread(&ThreadPoll::workThread,this));
                }
            }
            catch(...)
            {
                done=true;
                throw "Creat thread poll failed";
            }
        }

        ~ThreadPoll(){
            done=true;
        }

        template<typename FunctionType>
        void submit(FunctionType f)
        {
            workQueue.push(std::function<void()>(f));
        }

    private:
        size_t threadNum;
        std::atomic_bool done;
        std::vector<std::thread> threads;
        BlockingQueue<std::function<void()>> workQueue;

        void workThread(){
            while(!done){
                std::function<void()> task;
                if(workQueue.tryPop(task)){
                    task();
                }
                else{
                    std::this_thread::yield();
                }
            }
        }
    };
}

#endif //DURIANVER_THREADPOLL_H
