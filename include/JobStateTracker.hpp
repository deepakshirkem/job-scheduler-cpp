#pragma once
#include <unordered_map>
#include <mutex>

#include "JobState.hpp"

using JobID = int;

class JobStateTracker
{
    public:
        void setState(JobID id, JobState state);
        JobState getState(JobID id);

    private:
        std::unordered_map<JobID, JobState>  stateMap;
        std::mutex stateMutex;
};