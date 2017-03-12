//
// Created by justin on 3/12/17.
//

#include "logging.h"

namespace DURIANVER {

    std::shared_ptr<Logging> Logging::logInstance = NULL;

    void Logging::initLog(void) {
        std::time_t t = std::time(nullptr);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&t), "%F %T");
        std::string logFileName(ss.str());
        logFileName += " log.out";
        logFile.open(logFileName, std::ios::out | std::ios::app | std::ios::ate);
        if (logFile.bad()) {
            throw "Creat logFile failed";
        }
        logT = std::thread(&Logging::logThread, this);
    }

    void Logging::logThread() {
        while (!done) {
            if (logQueue.empty()) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            } else {
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
}
