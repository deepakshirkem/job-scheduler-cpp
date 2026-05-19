#include "Logger.hpp"
#include <iostream>
#include <chrono>
#include <ctime>


// Define the static member
std::mutex Logger::logMutex;
std::ofstream Logger::logFile;
bool Logger::fileLogging{false};

void Logger::init(const std::string& filePath)
{
    std::lock_guard<std::mutex> lock(logMutex);
    logFile.open(filePath, std::ios::app);

    if(logFile.is_open())
    {
        fileLogging = true;
        logFile <<  "═══════════════════════════════════\n";
        logFile << "  Schedulers starteed: " << getTimestamp() << "\n";
        logFile << "═══════════════════════════════════\n";
        std::cout << "Logging to files: " << filePath << std::endl;
    }
    else
    {
        std::cerr << "Failed to open log files: " << filePath << std::endl;
    }
}

void Logger::log(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(logMutex);
    std::string entry = "[" + getTimestamp() + ']' + msg;

    std::cout << entry << std::endl;

    if(fileLogging && logFile.is_open())
    {
        logFile << entry << "\n";
        logFile.flush();
    }
}

void Logger::close()
{
    std::lock_guard<std::mutex> lock(logMutex);
    if(logFile.is_open())
    {
        logFile << "═══════════════════════════════════\n";
        logFile << "  Schedulere stopped: " << getTimestamp() << "\n";
        logFile << "═══════════════════════════════════\n";
        logFile.close();
        fileLogging = false;
    }
}

std::string Logger::getTimestamp()
{
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::string ts = std::ctime(&t);
    ts.pop_back();
    return ts;
}