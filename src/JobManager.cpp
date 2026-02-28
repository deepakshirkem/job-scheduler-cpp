#include "JobManager.hpp"

JobManager::JobManager(ThreadSafeQueue<std::unique_ptr<Job>>& queue,
                       JobStateTracker& tracker)
    : jobQueue(queue), tracker(tracker) {}

void JobManager::submitJob(std::unique_ptr<Job> job)
{
    tracker.setState(job->getId(), JobState::PENDING);
    jobQueue.push(std::move(job));
}

JobState JobManager::getJobStatus(int id)
{
    return tracker.getState(id);
}