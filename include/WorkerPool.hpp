#pragma once
#include <vector>
#include <thread>
#include <atomic>
#include <memory>
#include "ThreadSafeQueue.hpp"
#include "Job.hpp"
#include "JobExecutor.hpp"

class WorkerPool
{
    public:
        WorkerPool(int numThreads, ThreadSafeQueue<Job*>& queue, JobExecutor& exec);
        void stop();
        
    private:
        std::vector<std::thread> workers;
        ThreadSafeQueue<Job*> jobQueue;
        JobExecutor& executor;
        std::atomic<bool> running;
};