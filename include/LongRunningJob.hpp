#pragma once
#include "Job.hpp"

class LongRunningJob : public Job
{
    public:
        LongRunningJob(JobID id, std::function<void()> task);
        bool isTerminationExpected() const override;

};