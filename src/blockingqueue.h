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
            std::lock_guard<std::mutex> lk(mt_);
            bdeque_.push_back(t);
            cond_.notify_one();
        }

        void push(T&& t){
            std::lock_guard<std::mutex> lk(mt_);
            bdeque_.push_back(std::move(t));
            cond_.notify_one();
        }

        size_t size(){
            std::lock_guard<std::mutex> lk(mt_);
            return bdeque_.size();
        }

        bool empty(){
            std::lock_guard<std::mutex> lk(mt_);
            return bdeque_.empty();
        }

        T pop(){
            std::unique_lock<std::mutex> lk(mt_);
            while(bdeque_.empty())
                cond_.wait(lk);
            T res(std::move(bdeque_.front()));
            bdeque_.pop_front();
            return res;
        }

        bool tryPop(T& t){
            std::lock_guard<std::mutex> lk(mt_);
            if(bdeque_.empty())
                return false;
            else{
                t=std::move(bdeque_.front());
                bdeque_.pop_front();
                return true;
            }
        }

        void swap(std::deque<T> &newQue){
            std::lock_guard<std::mutex> lk(mt_);
            bdeque_.swap(newQue);
        }

    private:
        mutable std::mutex mt_;
        std::condition_variable cond_;
        std::deque<T> bdeque_;
    };
}

