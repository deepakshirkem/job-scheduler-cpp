#include "WorkerPool.hpp"

WorkerPool::WorkerPool(int numThread, ThreadSafeQueue<std::unique_ptr<Job>>& queue, JobExecutor& exec)
            : jobQueue(queue), executor(exec), running(true)
        {
            for(int i=0; i < numThread; i++)
            {
                workers.emplace_back([this]()
                {
                    while(running)
                    {
                        std::unique_ptr<Job> job;
                        jobQueue.waitPop(job);
                        executor.executeJob(std::move(job));
                    }
                });
            }
        }

        void WorkerPool::stop()
        {
            running = false;

            for(auto& w: workers)
                if(w.joinable())
                    w.join();
        }