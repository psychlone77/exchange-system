#pragma once

#include <map>
#include <queue>
#include <vector>
#include <string>
#include "models/Order.h"
#include "models/ExecutionReportRow.h"

class OrderBook
{
private:
    // The instrument this specific order book instance handles
    Instrument instrumentType;

    // A nested structure to hold queued orders at a specific price level.
    using OrderQueue = std::queue<Order>;

    // Buy side: Higher the buy price, more attractive the order.
    std::map<double, OrderQueue, std::greater<double>> buySide;

    // Sell side: Lower the sell price, more attractive the order.
    std::map<double, OrderQueue, std::less<double>> sellSide;

    std::vector<ExecutionReportRow> matchBuyOrder(Order &order, const std::string &systemOrderId);
    std::vector<ExecutionReportRow> matchSellOrder(Order &order, const std::string &systemOrderId);

    // Helper to generate the unique Order ID required by the spec
    std::string generateOrderId();
    int orderCounter = 1;

public:
    // Constructor initializes the book for one specific flower type
    explicit OrderBook(Instrument inst) : instrumentType(inst) {}

    // The primary entry point for a validated order entering the book.
    std::vector<ExecutionReportRow> processOrder(Order &order);
};