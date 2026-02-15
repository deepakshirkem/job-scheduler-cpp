#include "Job.hpp"

Job::Job(JobID id, std::function<void()> task)
    : id(id), task(task) {}

void Job::execute()
{
    if(task)
        task();
}

Job::JobID Job::getId() const
{

    return id;
}