#pragma once
#include <iostream>
#include <mutex>
#include <string>

class Logger
{
    public:
        static void log(const std::string& msg)
        {
            std::lock_guard<std::mutex> lock(logMutex);
            std::cout <<msg << std::endl;
        }

        private:
            static std::mutex logMutex;
};