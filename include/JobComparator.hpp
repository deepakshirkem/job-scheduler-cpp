#pragma once
#include <memory>
#include "Job.hpp"

struct JobComparator
{
    bool operator()(const std::unique_ptr<Job>& a,
                    const std::unique_ptr<Job>& b) const
    {
        return a->getPriority() > b->getPriority();
    }
};
