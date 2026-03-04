#pragma
#include <atomic>
#include <csignal>
#include "Logger.hpp"

class SignalHandler
{
    public:
        static void setup();
        static bool isShutdownRequested();
        static void requstShutdown();
    
    private:
        static std::atomic<bool> shutdown_requested;
        static void handler(int signum);
};