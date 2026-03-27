#include "io/CSVExecutionWriter.h"
#include <fstream>
#include <iostream>
#include <iomanip>

// --- Internal Helper Functions ---
// These are marked 'static' so they remain internal to this translation unit,
// preventing naming collisions with other files.

static std::string instrumentToString(Instrument inst)
{
    switch (inst)
    {
    case Instrument::Rose:
        return "Rose";
    case Instrument::Lavender:
        return "Lavender";
    case Instrument::Lotus:
        return "Lotus";
    case Instrument::Tulip:
        return "Tulip";
    case Instrument::Orchid:
        return "Orchid";
    default:
        return "Invalid";
    }
}

static std::string statusToString(ExecStatus status)
{
    switch (status)
    {
    case ExecStatus::New:
        return "New";
    case ExecStatus::Rejected:
        return "Reject"; // Matched to slide 21's exact output
    case ExecStatus::Fill:
        return "Fill";
    case ExecStatus::PFill:
        return "PFill";
    default:
        return "Unknown";
    }
}

// --- Main Writer Implementation ---

void CSVExecutionWriter::writeExecutions(const std::vector<ExecutionReportRow> &reports)
{
    // Open the file in output mode (truncates existing file)
    std::ofstream outFile(filePath);

    if (!outFile.is_open())
    {
        std::cerr << "CRITICAL ERROR: Could not open output file at " << filePath << "\n";
        return;
    }

    // Write the exact comma-separated header required by the specifications
    outFile << "Order ID,Client Order ID,Instrument,Side,Exec Status,Quantity,Price,Reason,Transaction Time\n";

    // Set the output stream to format double values to exactly 2 decimal places (e.g., 55.00)
    outFile << std::fixed << std::setprecision(2);

    for (const auto &rep : reports)
    {
        // Build the CSV row
        outFile << rep.orderId << ","
                << rep.clientOrderId << ","
                << instrumentToString(rep.instrument) << ","
                << static_cast<int>(rep.side) << ","
                << statusToString(rep.status) << ","
                << rep.quantity << ","
                << rep.price << ",";

        // The reason column is only populated if the order was rejected.
        // If it's empty, we just leave nothing between the commas.
        if (!rep.reason.empty())
        {
            outFile << rep.reason;
        }

        outFile << "," << rep.transactionTime << "\n";
    }

    outFile.close();
    std::cout << "Successfully wrote execution reports to: " << filePath << "\n";
}