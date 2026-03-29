#include <iostream>
#include <string>
#include <chrono>
#include <memory>

#include "core/FlowerExchange.h"
#include "io/CSVOrderReader.h"
#include "io/CSVExecutionWriter.h"

int main(int argc, char *argv[])
{
    std::string inputFilePath = (argc > 1) ? argv[1] : "/home/nithika/Uni/Academic Projects/LSEG-CPP/flower-exchange/data/order_test.csv";

    std::string outputFilePath = (argc > 2) ? argv[2] : "/home/nithika/Uni/Academic Projects/LSEG-CPP/flower-exchange/data/execution_rep.csv";

    std::cout << "--- LSEG Flower Exchange Matching Engine ---\n";
    std::cout << "Input:  " << inputFilePath << "\n";
    std::cout << "Output: " << outputFilePath << "\n";
    std::cout << "Initializing system...\n";

    auto reader = std::make_shared<CSVOrderReader>(inputFilePath);
    auto writer = std::make_shared<CSVExecutionWriter>(outputFilePath);

    FlowerExchange exchange(reader, writer);

    auto startTime = std::chrono::high_resolution_clock::now();

    exchange.start();

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    std::cout << "Total Processing Time: " << duration.count() << " ms.\n";
    std::cout << "--------------------------------------------\n";

    return 0;
}