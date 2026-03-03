#pragma once
#include <iostream>
#include <mutex>
#include <string>
#include <fstream>

class Logger
{
    public:
        static void log(const std::string& msg);
        static void init(const std::string& filepath);
        static void close();

        private:
            static std::mutex logMutex;
            static std::ofstream logFile;
            static bool fileLogging;
            static std::string getTimestamp();
};