#pragma once
#include <memory>

#include "JobState.hpp"
#include "ThreadSafeQueue.hpp"
#include "JobStateTracker.hpp"
#include "JobExecutor.hpp"

class JobManager
{
    public:
        JobManager(ThreadSafeQueue<std::shared_ptr<Job>>& queue, JobStateTracker& tracker);      
        void submitJob(std::shared_ptr<Job> job);
        JobState getJobStatus(Job::JobID id);
        

    private:
        ThreadSafeQueue<std::shared_ptr<Job>>& jobQueue;
        JobStateTracker& tracker;

};