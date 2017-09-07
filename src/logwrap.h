#pragma once

#include "spdlog/spdlog.h"

#define LOGNAME "daily_logger"
#define SPDLOG_STR_H(x) #x
#define SPDLOG_STR_HELPER(x) SPDLOG_STR_H(x)

#define LOGDEBUG(...) spdlog::get(LOGNAME)->debug("[" __FILE__ ":" SPDLOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)
#define LOGINFO(...) spdlog::get(LOGNAME)->info("[" __FILE__ ":" SPDLOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)
#define LOGWARN(...) spdlog::get(LOGNAME)->warn("[" __FILE__ ":" SPDLOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)
#define LOGERR(...) spdlog::get(LOGNAME)->error("[" __FILE__ ":" SPDLOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)
