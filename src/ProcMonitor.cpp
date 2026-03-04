#include "ProcMonitor.hpp"
#include "Logger.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

ProcessStats ProcMonitor::readStats(int pid)
{
    ProcessStats stats;
    stats.pid = pid;
    stats.vmRSS = 0;
    stats.vmPeak = 0;
    stats.threads = 0;
    stats.utime = 0;
    stats.stime = 0;
    stats.state = "UNKNOWN";

    std::string statusPath = "/proc/" + std::to_string(pid) + "/status";
    std::ifstream statusFile(statusPath);

    if(statusFile.is_open())
    {
        std::string line;
        while(std::getline(statusFile, line))
        {
            if(line.find("VmRSS") != std::string::npos)
            {
                std::istringstream iss(line);
                std::string key, unit;
                iss >> key >> stats.vmRSS >> unit;
            }
            else if(line.find("VmPeak:") != std::string::npos)
            {
                std::istringstream iss(line);
                std::string key, unit;
                iss >> key >> stats.vmPeak >> unit;
            }
            else if(line.find("Threads:") != std::string::npos)
            {
                std::istringstream iss(line);
                std::string key;
                iss >> key >> stats.threads;
            }
        }
        statusFile.close();
    }

    std::string statPath = "/proc/" + std::to_string(pid) + "/stat";
    std::ifstream statFile(statPath);

    if(statFile.is_open())
    {
        std::string line;
        std::getline(statFile, line);
        size_t closePos = line.rfind(')');
        if(closePos != std::string::npos)
        {
            std::istringstream iss(line.substr(closePos + 2));
        
            std::string state;
            long skip;
        
            iss >> state;        // field 3 — state
            iss >> skip;         // field 4 — ppid
            iss >> skip;         // field 5 — pgrp
            iss >> skip;         // field 6 — session
            iss >> skip;         // field 7 — tty
            iss >> skip;         // field 8 — tpgid
            iss >> skip;         // field 9 — flags
            iss >> skip;         // field 10 — minflt
            iss >> skip;         // field 11 — cminflt
            iss >> skip;         // field 12 — majflt
            iss >> skip;         // field 13 — cmajflt
            iss >> stats.utime;  // field 14 — utime
            iss >> stats.stime;  // field 15 — stime
        
            stats.state = state;
            
        }
       
        statFile.close();
    }

 return stats;

}


ProcessStats ProcMonitor::readSelfStats()
{
    std::ifstream selfPid("/proc/self/stat");
    int pid;
    selfPid >> pid;
    selfPid.close();
    return readStats(pid);
}

void ProcMonitor::printStats(const ProcessStats& stats)
{
    Logger::log("═══════ Process Stats [PID: " + std::to_string(stats.pid) + "] ═══════");
    Logger::log("State      : " + stats.state);
    Logger::log("RAM Usage  : " + std::to_string(stats.vmRSS) + " kB");
    Logger::log("Peak RAM   : " + std::to_string(stats.vmPeak) + " kB");
    Logger::log("Threads    : " + std::to_string(stats.threads));
    Logger::log("User CPU   : " + std::to_string(stats.utime) + " ticks");
    Logger::log("System CPU : " + std::to_string(stats.stime) + " ticks");
    Logger::log("════════════════════════════════");
}