#include "Logger.hpp"
#include <iostream>


// Define the static member
std::mutex Logger::logMutex;

void Logger::log(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(logMutex);
    std::cout << msg << std::endl;
}