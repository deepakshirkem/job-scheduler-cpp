#pragma once
#include "Job.hpp"

class ShutdownJob : public Job
{
    public:
        ShutdownJob(JobID id, std::function<void()> task);
        bool isTerminationExpected() const override;
};