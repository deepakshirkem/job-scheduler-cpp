#pragma once
#include "Job.hpp"
#include <string>

class ShellJob : public Job
{
    public:
        ShellJob(JobID id, const std::string& command);
        void execute() override;
        bool isTerminationExpected() const override;
    
    private:
        std::string command;
};