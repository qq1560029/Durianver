//
// Created by justin on 3/4/17.
//

#pragma once

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <boost/core/noncopyable.hpp>

namespace DURIANVER {
    template <typename T>
    class BlockingQueue : boost::noncopyable{
    public:
        explicit BlockingQueue(){}

        void push(T& t){
            std::lock_guard<std::mutex> lk(mt);
            bdeque.push_back(t);
            cond.notify_one();
        }

        void push(T&& t){
            std::lock_guard<std::mutex> lk(mt);
            bdeque.push_back(std::move(t));
            cond.notify_one();
        }

        size_t size(){
            std::lock_guard<std::mutex> lk(mt);
            return bdeque.size();
        }

        bool empty(){
            std::lock_guard<std::mutex> lk(mt);
            return bdeque.empty();
        }

        T pop(){
            std::unique_lock<std::mutex> lk(mt);
            while(bdeque.empty())
                cond.wait(lk);
            T res(std::move(bdeque.front()));
            bdeque.pop_front();
            return res;
        }

        bool tryPop(T& t){
            std::lock_guard<std::mutex> lk(mt);
            if(bdeque.empty())
                return false;
            else{
                t=std::move(bdeque.front());
                bdeque.pop_front();
                return true;
            }
        }

        void swap(std::deque<T> &newQue){
            std::lock_guard<std::mutex> lk(mt);
            bdeque.swap(newQue);
        }

    private:
        mutable std::mutex mt;
        std::condition_variable cond;
        std::deque<T> bdeque;
    };
}

