#pragma once
#include <functional>

class Job
{
    public:
        using JobID = int;
        Job(JobID id, std::function<void()> task);
        void execute();
        JobID getId() const;

    private:
        JobID id;
        std::function<void()> task;
};