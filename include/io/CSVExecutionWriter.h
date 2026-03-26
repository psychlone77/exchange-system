#pragma once

#include <string>
#include "interfaces/IExecutionWriter.h"

class CSVExecutionWriter : public IExecutionWriter
{
private:
    std::string filePath;

public:
    explicit CSVExecutionWriter(std::string path) : filePath(std::move(path)) {}

    void writeExecutions(const std::vector<ExecutionReportRow> &reports) override;
};