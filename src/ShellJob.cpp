#include "ShellJob.hpp"
#include "Logger.hpp"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdexcept>

ShellJob::ShellJob(JobID id, const std::string& command) : Job(id, nullptr), command(command) {}

bool ShellJob::isTerminationExpected() const
{
    return true;
}

void ShellJob::execute()
{
    Logger::log("Executing shell command: " + command);
    pid_t pid = fork();

    if(pid < 0)
    {
        throw std::runtime_error("fork() failed for command: " + command);
    }
    else if (pid == 0)
    {
        execl("/bin/sh", "sh", "-c", command.c_str(), nullptr);
        _exit(1);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);

        if(WIFEXITED(status))
        {
            int exitCode = WEXITSTATUS(status);
            if(exitCode != 0)
            {
                throw std::runtime_error("Command failed with exit code: " + std::to_string(exitCode));
                Logger::log("Command completed: " + command);
            } 
            else
            {
                Logger::log("Command completed successfully: " + command);

            }          
        }
         else
        {
            throw std::runtime_error("Command terminated abnormally: " + command);
        }
    }
}