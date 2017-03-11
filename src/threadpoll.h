//
// Created by justin on 3/1/17.
//

#pragma once

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
            try
            {
                for(size_t i=0;i<threadNum;++i)
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
            workQueue.push(f);
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

