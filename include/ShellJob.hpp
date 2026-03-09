#pragma once
#include "Job.hpp"
#include <string>

class ShellJob : public Job
{
    public:
        ShellJob(JobID id, const std::string& command, int priority = 3);
        void execute() override;
        bool isTerminationExpected() const override;
    
    private:
        std::string command;
};