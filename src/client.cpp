#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>

#define SOCKET_PATH "/tmp/job-scheduler.sock"

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout << "Usage: scheduler-client <command>" << std::endl;
        std::cout << "Commands: STATUS, SHUTDOWN, SUBMIT" << std::endl;
        return 1;
    }

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(fd < 0)
    {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if(connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        std::cerr << "Failed to connect to scheduler — is it running?" << std::endl;
        close(fd);
        return 1;       
    }

    std::string command = argv[1];
    write(fd, command.c_str(), command.size());

    char buffer[1024] = {0};
    int bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if(bytesRead > 0)
    {
        std::cout << "Response: " << std::string(buffer, bytesRead) << std::endl; 
    }

    close(fd);
    return 0;
}