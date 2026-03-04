#pragma once
#include <string>

struct ProcessStats
{
    int pid;
    std::string state;
    long vmRSS;
    long vmPeak;
    int threads;
    long utime;
    long stime;
};

class ProcMonitor
{
    public:
    static ProcessStats readStats(int pid);
    static ProcessStats readSelfStats();
    static void printStats(const ProcessStats& stats);
};