#include "JobManager.hpp"

JobManager::JobManager(ThreadSafeQueue<Job> queue,
                       JobStateTracker& tracker)
    : jobQueue(queue), tracker(tracker) {}

void JobManager::submitJob(Job* job)
{
    tracker.setState(job->getId(), JobState::PENDING);
    jobQueue.push(job);
}

JobState JobManager::getJobStatus(int id)
{
    return tracker.getState(id);
}