#include "ShutdownJob.hpp"

ShutdownJob::ShutdownJob(JobID id, std::function<void()> task) : Job(id, task, 3)
{

}

bool ShutdownJob::isTerminationExpected() const
{
    return true;

}