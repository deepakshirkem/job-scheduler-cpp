#include "JobStateTracker.hpp"

void JobStateTracker::setState(JobID id, JobState state)
{
    std::lock_guard<std::mutex> lock(stateMutex);
    stateMap[id] = state;
}

JobState JobStateTracker::getState(JobID id)
{
    std::lock_guard<std::mutex> lock(stateMutex);

    if(stateMap.find(id) != stateMap.end())
        return stateMap[id];

    return JobState::FAILED;
}