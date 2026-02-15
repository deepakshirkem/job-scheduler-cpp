#pragma once

#include "JobState.hpp"  // MUST BE FIRST, before any other project headers

#include <unordered_map>
#include <mutex>

#include "Job.hpp"

class JobStateTracker
{
public:
    void setState(Job::JobID id, JobState state);
    JobState getState(Job::JobID id);

private:
    std::unordered_map<Job::JobID, JobState> stateMap;
    std::mutex stateMutex;
};