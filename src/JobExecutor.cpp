#include "JobExecutor.hpp"
#include "../include/JobState.hpp"

JobExecutor::JobExecutor(JobStateTracker& tracker) : tracker(tracker) {}

void JobExecutor::executeJob(std::shared_ptr<Job> job)
{
    int id = job->getId();

    tracker.setState(id, JobState::RUNNING);

    try 
    {
        job->execute();
        tracker.setState(id, JobState::COMPLETED);
    }
    catch(...)
    {
        tracker.setState(id, JobState::FAILED);
        Logger::log("Job Failed:" + std::to_string(id));
    }
}