#pragma once
#include <string>

enum class JobState
{
    PENDING,
    RUNNING,
    COMPLETED,
    FAILED,
    CANCELLED,
    RETRYING  
};

inline std::string jobStateToString(JobState state)
{
    switch(state)
    {
        case JobState::PENDING:     return "PENDING";
        case JobState::RUNNING:     return "RUNNING";
        case JobState::COMPLETED:   return "COMPLETED";
        case JobState::FAILED:      return "FAILED";
        case JobState::CANCELLED:   return "CANCELLED";
        case JobState::RETRYING:    return "RETRYING";
        default:                    return "UNKNOWN";
    }
    
}