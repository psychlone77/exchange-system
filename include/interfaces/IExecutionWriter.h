#pragma once

#include <vector>
#include "models/ExecutionReportRow.h"

class IExecutionWriter
{
public:
    virtual ~IExecutionWriter() = default;

    // Pure virtual function to handle the output of execution reports
    virtual void writeExecutions(const std::vector<ExecutionReportRow> &reports) = 0;
};