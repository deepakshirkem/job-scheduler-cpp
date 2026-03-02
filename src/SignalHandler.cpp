#include "SignalHandler.hpp"

std::atomic<bool> SignalHandler::shutdown_requested{false};

void SignalHandler::setup()
{
    signal(SIGTERM, SignalHandler::handler);
    signal(SIGINT, SignalHandler::handler);
    Logger::log("Signal handler registered for SIGTERM and SIGINT");
}

bool SignalHandler::isShutdownRequested()
{
    return shutdown_requested.load();
}

void SignalHandler::handler(int signum)
{
    if(shutdown_requested.load())
    {
        return;
    }
    if(signum == SIGTERM)
    {
        Logger::log("SIGTERM received — graceful shutdown initiated");
    }
    else if ( signum == SIGINT)
    {
        Logger::log("SIGINT received — graceful shutdown initiated");
    }

    shutdown_requested = true;
}