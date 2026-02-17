#include "JobExecutor.hpp"
#include "Job.hpp"
#include <thread>
#include <chrono>

JobExecutor::JobExecutor(JobStateTracker& tracker) : tracker(tracker) {}

void JobExecutor::executeJob(std::shared_ptr<Job> job)
{
    int id = job->getId();


    try 
    {
        tracker.setState(id, JobState::RUNNING);
        job->execute();
        tracker.setState(id, JobState::COMPLETED);
    }
    catch(...)
    {
        if(job->canRetry())
        {
            tracker.setState(id, JobState::RETRYING);
            job->incrementRetry();

            Logger::log("Retrying Job: " + std::to_string(id) + 
                        " Attempt: " + std::to_string(job->getRetryCount()));

            std::this_thread::sleep_for(std::chrono::seconds(1));
            executeJob(job);
        }
        else
        {
            tracker.setState(id, JobState::FAILED);
            Logger::log("Job Permanetly Failed: " + std::to_string(id));
        }
    }
}