#include "SocketServer.hpp"
#include "Logger.hpp"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <cstring>

SocketServer::SocketServer(const std::string& socketPath, CommandHandler handler)
    : socketPath(socketPath), commandHandler(handler) {}

SocketServer::~SocketServer()
{
    stop();
}

void SocketServer::start()
{
    running = true;
    serverThread = std::thread(&SocketServer::listen, this);
    Logger::log("Socket server started at:" + socketPath);
}

void SocketServer::stop()
{
    running = false;
    shutdown(serverFd, SHUT_RDWR);
    close(serverFd);
    unlink(socketPath.c_str());
    if(serverThread.joinable())
    {
        serverThread.join();
    }   

    Logger::log("Socket server stopped");
}

void SocketServer::listen()
{
    serverFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(serverFd < 0)
    {
        Logger::log("Failed to create socket");
        return;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socketPath.c_str(), sizeof(addr.sun_path) -1);

    unlink(socketPath.c_str());

    if(bind(serverFd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        Logger::log("Failed to listen on socket");
        close(serverFd);
        return;
    }

    if(::listen(serverFd, 5) < 0)
    {
        Logger::log("Failed to listen on socket");
        close(serverFd);
        return;
    }

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(serverFd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    Logger::log("Socket listening for commands...");

    while(running)
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(serverFd, &readfds);

        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        
        int ready = select(serverFd + 1, &readfds, nullptr, nullptr, &timeout);

        if(ready < 0)
        {
            if(running) Logger::log("select() error");
            break;
           
        }
        else if(ready == 0)
        {
            continue;
        }

        int clientFd = accept(serverFd, nullptr, nullptr);
        if(clientFd < 0)
        {
            continue;
        }
   
        handleClient(clientFd);
        close(clientFd);
    }

    close(serverFd);
}

void SocketServer::handleClient(int clientFd)
{
    char buffer[1024] = {0};
    int bytesRead = read(clientFd, buffer, sizeof(buffer) - 1);

    if(bytesRead > 0)
    {
        std::string command(buffer, bytesRead);
        while(!command.empty() && (command.back() == '\n' || command.back() == '\r'))
        {
            command.pop_back();
        }

        Logger::log("Received command: " + command);

        std::string response = commandHandler(command);
        write(clientFd, response.c_str(), response.size());
    }
    else
    {
        Logger::log("Read failed — bytesRead: " + std::to_string(bytesRead) + 
                   " errno: " + std::to_string(errno));
    }
}