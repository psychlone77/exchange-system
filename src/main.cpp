#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include "core/FlowerExchange.h"
#include "io/CSVOrderReader.h"
#include "io/CSVExecutionWriter.h"

int main(int argc, char *argv[])
{
    // Allow overriding file paths via command line, defaulting to the data folder
    std::string inputFilePath = (argc > 1) ? argv[1] : "/home/nithika/Uni/Academic Projects/LSEG-CPP/flower-exchange/data/order2.csv";
    std::string outputFilePath = (argc > 2) ? argv[2] : "/home/nithika/Uni/Academic Projects/LSEG-CPP/flower-exchange/data/execution_rep.csv";

    std::cout << "--- LSEG Flower Exchange Matching Engine ---\n";
    std::cout << "Input:  " << inputFilePath << "\n";
    std::cout << "Output: " << outputFilePath << "\n";
    std::cout << "Initializing system...\n";

    // 1. Initialize our decoupled components using the interfaces
    CSVOrderReader reader(inputFilePath);
    CSVExecutionWriter writer(outputFilePath);
    FlowerExchange exchange;

    // Start the performance timer
    auto startTime = std::chrono::high_resolution_clock::now();

    // 2. Read all orders into memory
    std::vector<Order> orders = reader.readOrders();
    if (orders.empty())
    {
        std::cerr << "No orders to process. Exiting.\n";
        return 1;
    }
    std::cout << "Successfully ingested " << orders.size() << " orders.\n";
    std::cout << "Matching engine started...\n";

    // 3. Process orders and collect execution reports
    std::vector<ExecutionReportRow> allExecutionReports;
    // Pre-allocate memory assuming at least one report per order to prevent reallocation overhead
    allExecutionReports.reserve(orders.size() * 2);

    for (Order &order : orders)
    {
        std::vector<ExecutionReportRow> reports = exchange.processOrder(order);

        // Append the generated reports for this specific order into our master list
        allExecutionReports.insert(allExecutionReports.end(), reports.begin(), reports.end());
    }

    // 4. Write out the results
    writer.writeExecutions(allExecutionReports);

    // Stop the timer and calculate duration
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    std::cout << "Execution complete. Generated " << allExecutionReports.size() << " reports.\n";
    std::cout << "Total Processing Time: " << duration.count() << " ms.\n";
    std::cout << "--------------------------------------------\n";

    return 0;
}