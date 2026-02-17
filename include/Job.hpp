#pragma once
#include <functional>

class Job
{
    public:
        using JobID = int;

        Job(JobID id, std::function<void()> task, int maxRetries = 2 );
            

        void execute();

        JobID getId() const;

        bool canRetry() const { return retryCount < maxRetries; }

        void incrementRetry() { retryCount++; } 

        int getRetryCount() const { return retryCount; }
        

    private:
        JobID id;
        std::function<void()> task;
        int maxRetries;
        int retryCount;
};