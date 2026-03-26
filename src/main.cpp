#include <iostream>
#include <vector>
#include <string>
#include <iomanip> // For formatting price output

// Include using the updated directory structure
#include "models/Order.h"
#include "io/CSVOrderReader.h"

std::string instrumentToString(Instrument inst)
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

std::string sideToString(Side side)
{
    switch (side)
    {
    case Side::Buy:
        return "1 (Buy)";
    case Side::Sell:
        return "2 (Sell)";
    default:
        return "Unknown";
    }
}

// --- Main Application Entry Point ---

int main()
{
    // We assume you have created a 'data' folder in your project root
    // and placed the sample 'orders.csv' inside it.
    std::string testFilePath = "/home/nithika/Uni/Academic Projects/LSEG-CPP/flower-exchange/data/order2.csv";

    std::cout << "Initializing Flower Exchange Data Ingestion Test...\n";
    std::cout << "Attempting to read: " << testFilePath << "\n\n";

    // Instantiate our reader
    CSVOrderReader reader(testFilePath);

    // Fetch the orders into our high-performance vector
    std::vector<Order> orders = reader.readOrders();

    if (orders.empty())
    {
        std::cout << "No orders were parsed. Please check if the file exists and is formatted correctly.\n";
        return 1;
    }

    std::cout << "Successfully parsed " << orders.size() << " orders. Displaying results:\n";
    std::cout << std::string(80, '-') << "\n";

    // Setup console output formatting for price
    std::cout << std::fixed << std::setprecision(2);

    // Iterate through the parsed orders and print them
    for (const auto &order : orders)
    {
        std::cout << "Client Order ID: " << order.clientOrderId << "\t | "
                  << "Instrument: " << instrumentToString(order.instrument) << "\t | "
                  << "Side: " << sideToString(order.side) << "\t | "
                  << "Qty: " << order.quantity << "\t | "
                  << "Price: " << order.price << "\n";
    }

    std::cout << std::string(80, '-') << "\n";
    std::cout << "Data ingestion test complete.\n";

    return 0;
}