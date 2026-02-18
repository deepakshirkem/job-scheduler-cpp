#include "LongRunningJob.hpp"

LongRunningJob::LongRunningJob(JobID id, std::function<void()> task): Job(id, task)
{

}

bool LongRunningJob::isTerminationExpected() const
{
    return false;
}