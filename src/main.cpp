#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

#include "ThreadSafeQueue.hpp"
#include "Job.hpp"
#include "JobState.hpp"
#include "JobStateTracker.hpp"
#include "JobExecutor.hpp"
#include "WorkerPool.hpp"
#include "JobManager.hpp"
#include "ShutdownJob.hpp"
#include "SignalHandler.hpp"
#include "ProcMonitor.hpp"
#include "SocketServer.hpp"
#include "ShellJob.hpp"
#include <unistd.h>

#define SOCKET_PATH "/tmp/job-scheduler.sock"


int main()
{
    SignalHandler::setup();
    Logger::init("jobscheduler.log");

    int schdulerPid = getpid();
    Logger::log("Scheduler started with PID: " + std::to_string(schdulerPid));
    ThreadSafeQueue<std::unique_ptr<Job>> jobQueue;
    JobStateTracker tracker;
    JobExecutor executor(tracker);
    JobManager manager(jobQueue, tracker);
    WorkerPool pool(3, jobQueue, executor);

    Logger::log("Initial resource usagee:");
    ProcMonitor::printStats(ProcMonitor::readStats(schdulerPid));

    std::atomic<int> jobCounter{1};

    SocketServer socketServer(SOCKET_PATH, [&manager, &jobQueue, &jobCounter](const std::string& cmd) -> std::string
    {
        if(cmd == "STATUS")
        {
            int total = jobCounter.load() - 1;
            if(total == 0)
            {
                return "No Jobs Submitted yet\n";
            }

            std::string response = "Job Status:\n";
            for(int i=1; i <= total; i++)
            {
                JobState state = manager.getJobStatus(i);
                response += "Job " + std::to_string(i) + ": " + jobStateToString(state) + "\n";
            }
            return response;
        }
        else if (cmd.size() > 7 && cmd.substr(0, 6) == "SUBMIT")
        {
            std::string rest = cmd.substr(7);

            int priority = 3;
            size_t spacePos = rest.find(' ');

            if(spacePos != std::string::npos)
            {
                std::string prioritystr = rest.substr(0, spacePos);

                bool isNumber = !prioritystr.empty() && std::all_of(prioritystr.begin(), prioritystr.end(), ::isdigit);

                if(isNumber)
                {
                    priority = std::stoi(prioritystr);
                    rest = rest.substr(spacePos + 1);
                }
            }
            int newId = jobCounter.fetch_add(1);
            auto job = std::make_unique<ShellJob>(newId, rest, priority);
            manager.submitJob(std::move(job));
            Logger::log("Job " + std::to_string(newId) + " submitted with priority " + std::to_string(priority) + ": " + rest + "\n");

            return "Job " + std::to_string(newId) + " submitted with priority" + std::to_string(priority) + "\n";
        }
        else if(cmd == "SHUTDOWN")
        {
            Logger::log("Shutdown requested via socket");
            raise(SIGTERM);
            return "Shutdown Initiated\n";
        }
        else
        {
            return "Unknown command: " + cmd + "\nAvailable: STATUS, SUBMIT <cmd> , SHUTDOWN\n";
        }
    });

    socketServer.start();
    Logger::log("Scheduler ready — waiting for jobs via scheduler-client");
    Logger::log("Commands: SUBMIT <cmd>, STATUS, SHUTDOWN");

    while(!SignalHandler::isShutdownRequested())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    Logger::log("Final resource usage:");
    ProcMonitor::printStats(ProcMonitor::readStats(schdulerPid));

    int total = jobCounter.load() - 1;
    if(total > 0)
    {
        Logger::log("Final job states:");
        for(int i=1; i <= total; i++)
        {
            JobState state = manager.getJobStatus(i);
            std::cout << "Job " << i << " status: " << jobStateToString(state) << std::endl;
        }
    }

    Logger::log("Shutting down waiting for running jobs to complete");
    socketServer.stop();
    jobQueue.shutdown();
    pool.stop();
    Logger::log("Scheduler exited cleanly");
    Logger::close();
    return 0;
}