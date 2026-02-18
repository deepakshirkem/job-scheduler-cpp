#include "Job.hpp"

Job::Job(JobID id, std::function<void()> task, int maxRetries)
    : id(id), task(task), maxRetries(maxRetries), retryCount(0) {}

void Job::execute()
{
    if(task)
        task();
}

Job::~Job()
{
    
}

Job::JobID Job::getId() const
{

    return id;
}