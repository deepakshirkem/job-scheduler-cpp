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


int main()
{
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

    std::this_thread::sleep_for(std::chrono::seconds(10));

    for(int i=1; i <= 5; i++)
    {
        JobState state = manager.getJobStatus(i);
        std::cout << "Job " << i << " status: " << static_cast<int>(state) << std::endl;
    }

    pool.stop();
    return 0;
}