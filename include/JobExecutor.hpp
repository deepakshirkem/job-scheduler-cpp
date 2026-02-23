#pragma once

#include "JobState.hpp"  // FIRST

#include <memory>
#include "Job.hpp"
#include "JobStateTracker.hpp"
#include "Logger.hpp"

class JobExecutor
{
public:
    JobExecutor(JobStateTracker& tracker);
    void executeJob(Job* job);

private:
    JobStateTracker& tracker;
};