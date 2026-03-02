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
#include "LongRunningJob.hpp"
#include "SignalHandler.hpp"


int main()
{
    SignalHandler::setup();
    ThreadSafeQueue<std::unique_ptr<Job>> jobQueue;
    JobStateTracker tracker;
    JobExecutor executor(tracker);
    JobManager manager(jobQueue, tracker);
    WorkerPool pool(3, jobQueue, executor);

    for(int i=1; i <=5; i++)
    {
        std::unique_ptr<Job> job;

        if(i == 2)
        {
            job = std::make_unique<ShutdownJob>(i, [i]()
            {
                Logger::log("Excuting Shutdown Jobs:" + std::to_string(i));
                std::this_thread::sleep_for(std::chrono::seconds(2));

                
            }); 
        }
         else
            {
                job = std::make_unique<LongRunningJob>(i, [i]
                {
                    Logger::log("Executing Long Running Job: " + std::to_string(i));
                    std::this_thread::sleep_for(std::chrono::seconds(2));

                    if(i == 3)
                    {
                        throw std::runtime_error("Crash");
                    }
                });
            }
       
        manager.submitJob(std::move(job));
    }

    Logger::log("Scheduler running — press Ctrl+C to shutdow");
    while(!SignalHandler::isShutdownRequested())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        bool alldone = true;
        for(int i=1; i <= 5; i++)
        {
            JobState state = manager.getJobStatus(i);
            if(state == JobState::PENDING || state == JobState::RUNNING || state == JobState::RETRYING)
            {
                alldone = false;
                break;
            }
        }

        if(alldone)
        {
            Logger::log("All jobs completed naturally");
            break;
        }
    }

    Logger::log("Shutting down — waiting for running jobs to complete");
    pool.stop();
    Logger::log("ll jobs completed — scheduler exited clean");

    for(int i=1; i <= 5; i++)
    {
        JobState state = manager.getJobStatus(i);
        std::cout << "Job " << i << " status: " << jobStateToString(state) << std::endl;
    }

    return 0;
}