#pragma once
#include <string>
#include <thread>
#include <atomic>
#include <functional>

class SocketServer
{
    public:

        using CommandHandler = std::function<std::string(const std::string&)>;
        SocketServer(const std::string& socketPath, CommandHandler handler);
        ~SocketServer();

        void start();
        void stop();

    private:
        std::string socketPath;
        CommandHandler commandHandler;
        std::atomic<bool> running{false};
        std::thread serverThread;
        int serverFd{-1};

        void listen();
        void handleClient(int clientFd);
};