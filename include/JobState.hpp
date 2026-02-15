#pragma once

enum class JobState
{
    PENDING,
    RUNNING,
    COMPLETED,
    FAILED,
    CANCELLED,
    RETRYING  
};