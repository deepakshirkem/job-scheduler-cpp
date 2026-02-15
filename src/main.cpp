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


int main()
{
    ThreadSafeQueue<std::shared_ptr<Job>> jobQueue;
    JobStateTracker tracker;
    JobExecutor executor(tracker);
    JobManager manager(jobQueue, tracker);

    WorkerPool pool(3, jobQueue, executor);

    for(int i=1; i <=5; i++)
    {
        auto job = std::make_shared<Job>(i, [i]()
        {
            std::cout << "Executing Job " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));

            if(i == 3)
            {
                throw std::runtime_error("Failure");
            }
        });

        manager.submitJob(job);
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));

    for(int i=1; i <= 5; i++)
    {
        JobState state = manager.getJobStatus(i);
        std::cout << "Job " << i << "status: " << static_cast<int>(state) << std::endl;
    }

    pool.stop();
    return 0;
}