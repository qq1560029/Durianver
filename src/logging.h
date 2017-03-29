//
// Created by justin on 3/6/17.
//

#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <thread>
#include <boost/core/noncopyable.hpp>
#include <atomic>
#include "blockingqueue.h"

namespace DURIANVER {

    class Logging : boost::noncopyable {
    public:
        Logging() : done(false){
            initLog();
        }

        ~Logging(){
            done=true;
            logT.join();
        }

        static std::shared_ptr<Logging> getLog(){
            if(!logInstance)
                logInstance=std::make_shared<Logging>();
            return logInstance;
        }

        void operator<<(std::string &&log) {
            std::time_t t = std::time(nullptr);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&t), "%F %T");
            ss<<" "<<log;
            logQueue.push(ss.str());
        }

    private:
        std::ofstream logFile;
        BlockingQueue<std::string> logQueue;
        std::atomic_bool done;
        std::thread logT;
        static std::shared_ptr<Logging> logInstance;

        void initLog(void);

        void logThread(void);
    };

    static class LogINFO{
    public:
        void operator<<(std::string &&log){
            *Logging::getLog()<<"[INFO]"+log+"\n";
        }
    }LOGINFO;

    static class LogERR{
    public:
        void operator<<(std::string &&log){
            *Logging::getLog()<<"[ERROR]"+log+"\n";
        }
    }LOGERR;

    static class LogWARN{
    public:
        void operator<<(std::string &&log){
            *Logging::getLog()<<"[WARNING]"+log+"\n";
        }
    }LOGWARN;
//
//    class logBase{
//    public:
//        logBase& operator<<(std::string &&log){
//            logTmp+=log;
//            return *this;
//        }
//        logBase& operator<<(int &&log){
//            std::stringstream ss;
//            ss<<log;
//            logTmp+=ss.str();
//            return *this;
//        }
//
//        void initLog(std::string &&head){
//            logTmp+=head;
//        }
//
//        void exitLog(){
//            *Logging::getLog()<<logTmp+"\n";
//        }
//    private:
//        std::string logTmp;
//    };
//
//    class logIn:public logBase{
//    public:
//        logIn(){
//            initLog("[INFO]");
//        }
//        ~logIn(){
//            exitLog();
//        }
//    };

}
