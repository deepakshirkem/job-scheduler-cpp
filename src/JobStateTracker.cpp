#include "JobStateTracker.hpp"

void JobStateTracker::setState(int id, JobState state)
{
    std::lock_guard<std::mutex> lock(stateMutex);
    stateMap[id] = state;
}

JobState JobStateTracker::getState(int id)
{
    std::lock_guard<std::mutex> lock(stateMutex);

    if(stateMap.find(id) != stateMap.end())
        return stateMap[id];

    return JobState::FAILED;
}