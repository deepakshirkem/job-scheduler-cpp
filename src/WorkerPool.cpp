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
                        if(!jobQueue.waitPop(job))
                        {
                            break;
                        }
                        executor.executeJob(std::move(job));
                    }
                });
            }
        }

        void WorkerPool::stop()
        {
            running = false;
            jobQueue.shutdown();

            for(auto& w: workers)
                if(w.joinable())
                    w.join();
        }