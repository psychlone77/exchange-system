#pragma once

#include <map>
#include <vector>
#include <string>
#include "models/Order.h"
#include "OrderBook.h"
#include "models/ExecutionReportRow.h"

class FlowerExchange
{
private:
    // The exchange app maintains one order book for each Instrument (flower type).
    std::map<Instrument, OrderBook> orderBooks;

    int currentOrderId = 1;

    std::string generateOrderId();

public:
    FlowerExchange();

    std::vector<ExecutionReportRow> processOrder(Order &order);
};