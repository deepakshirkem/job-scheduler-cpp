#pragma once

#include "JobState.hpp"  // FIRST

#include <memory>
#include "ThreadSafeQueue.hpp"
#include "JobStateTracker.hpp"
#include "JobExecutor.hpp"

class JobManager
{
public:
    JobManager(ThreadSafeQueue<std::unique_ptr<Job>>& queue, JobStateTracker& tracker);      
    void submitJob(std::unique_ptr<Job> job);
    JobState getJobStatus(int id);

private:
    ThreadSafeQueue<std::unique_ptr<Job>>& jobQueue;
    JobStateTracker& tracker;
};