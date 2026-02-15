#include "JobManager.hpp"

JobManager::JobManager(ThreadSafeQueue<std::shared_ptr<Job>>& queue, JobStateTracker& tracker)
            : jobQueue(queue), tracker(tracker) {}

void JobManager::submitJob(std::shared_ptr<Job> job)
{
    tracker.setState(job->getId(), JobState::PENDING);
    jobQueue.push(job);
}

JobState JobManager::getJobStatus(JobID id)
{
    return tracker.getState(id);
}