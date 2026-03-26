#pragma once

#include <string>
#include <vector>
#include "Order.h"
#include "models/ExecutionReportRow.h"

class CSVHandler
{
public:
    static std::vector<Order> readOrders(const std::string &inputFilePath);

    static void writeExecutionReports(const std::string &outputFilePath, const std::vector<ExecutionReportRow> &reports);

    static std::string getCurrentTransactionTime();

private:
    static Instrument parseInstrument(const std::string &instStr);
    static std::string instrumentToString(Instrument inst);

    static Side parseSide(const std::string &sideStr);
    static std::string sideToString(Side side);

    static std::string statusToString(ExecStatus status);
};