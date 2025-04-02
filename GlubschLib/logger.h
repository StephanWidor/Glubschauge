#pragma once
#ifdef ANDROID
#include <android/log.h>
#else
#include <iostream>
#endif
#include <format>
#include <string>

namespace logger {

struct Out
{
#ifdef ANDROID
    Out &operator<<(const char *text)
    {
        __android_log_write(ANDROID_LOG_INFO, "[log info] ", text);
        return *this;
    }
    Out &operator<<(const std::string &text)
    {
        __android_log_write(ANDROID_LOG_INFO, "[log info] ", text.c_str());
        return *this;
    }
    template<typename WhatEver>
    Out &operator<<(const WhatEver &whatEver)
    {
        __android_log_write(ANDROID_LOG_INFO, "[log info] ", std::to_string(whatEver).c_str());
        return *this;
    }
#else
    template<typename WhatEver>
    Out &operator<<(const WhatEver &whatEver)
    {
        std::cout << "[log info] " << whatEver << std::endl;
        return *this;
    }
#endif
};

static Out out;

}    // namespace logger
