//
// Created by justin on 3/6/17.
//

#ifndef DURIANVER_LOGGING_H
#define DURIANVER_LOGGING_H

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

    class Logging;
    std::shared_ptr<Logging> logger;
    std::once_flag logInitOnce;

    void logInit(){
        logger=std::make_shared<Logging>();
    }

    class Logging : boost::noncopyable {
    public:
        Logging() : done(false){
            std::call_once(logInitOnce,&Logging::initLog,this);
        }

        ~Logging(){
            done=true;
            logT.join();
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

        void initLog(void){
            std::time_t t = std::time(nullptr);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&t), "%F %T");
            std::string logFileName(ss.str());
            logFileName += " log.out";
            logFile.open(logFileName, std::ios::out | std::ios::app | std::ios::ate);
            if (logFile.bad()) {
                throw "Creat logFile failed";
            }
            logT=std::thread(&Logging::logThread,this);
        }

        void logThread(){
            while(!done) {
                if(logQueue.empty()){
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                else {
                    std::deque<std::string> swapQue;
                    logQueue.swap(swapQue);
                    for (auto i:swapQue) {
                        logFile << i;
                    }
                    logFile.flush();
                }
            }
            std::deque<std::string> swapQue;
            logQueue.swap(swapQue);
            for (auto i:swapQue) {
                logFile << i;
            }
            logFile.close();
        }
    };

    class logINFO{
    public:
        void operator<<(std::string &&log){
            *logger<<"[INFO] "+log+"\n";

        }
    }logINFO;

    class logERROR{
    public:
        void operator<<(std::string &&log){
            *logger<<"[ERROR] "+log+"\n";
        }
    }logERROR;

    class logWARN{
    public:
        void operator<<(std::string &&log){
            *logger<<"[WARNING] "+log+"\n";
        }
    }logWARN;
}

#endif //DURIANVER_LOGGING_H
