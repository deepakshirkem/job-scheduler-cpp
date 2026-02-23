#pragma once

#include "JobState.hpp"  // FIRST

#include <memory>
#include "ThreadSafeQueue.hpp"
#include "JobStateTracker.hpp"
#include "JobExecutor.hpp"

class JobManager
{
public:
    JobManager(ThreadSafeQueue<Job*> queue, JobStateTracker& tracker);      
    void submitJob(Job* job);
    JobState getJobStatus(int id);

private:
    ThreadSafeQueue<std::shared_ptr<Job>>& jobQueue;
    JobStateTracker& tracker;
};