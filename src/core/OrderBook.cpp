#include "core/OrderBook.h"
#include "io/Utils.h"
#include <algorithm>

std::vector<ExecutionReportRow> OrderBook::processOrder(Order &order, const std::string &systemOrderId)
{
    if (order.side == Side::Buy)
    {
        return matchBuyOrder(order, systemOrderId);
    }
    else
    {
        return matchSellOrder(order, systemOrderId);
    }
}

std::vector<ExecutionReportRow> OrderBook::matchBuyOrder(Order &order, const std::string &systemOrderId)
{
    std::vector<ExecutionReportRow> reports;
    int initialQty = order.quantity;

    // Keep matching as long as the aggressive order has quantity and there are resting sell orders
    while (order.quantity > 0 && !sellSide.empty())
    {
        auto bestSellIt = sellSide.begin();
        double bestSellPrice = bestSellIt->first;

        // If the best available sell price is higher than what we are willing to buy for, stop matching.
        if (bestSellPrice > order.price)
        {
            break;
        }

        // Get the queue of orders at this price level
        OrderQueue &restingOrders = bestSellIt->second;
        Order &restingOrder = restingOrders.front(); // Time Priority: First In, First Out

        // Determine trade size
        int tradeQty = std::min(order.quantity, restingOrder.quantity);

        // The execution price is determined by the resting order's price
        double tradePrice = bestSellPrice;

        // Update quantities
        order.quantity -= tradeQty;
        restingOrder.quantity -= tradeQty;

        // Generate Execution Report for the Aggressive Order
        ExecStatus aggStatus = (order.quantity == 0) ? ExecStatus::Fill : ExecStatus::PFill;
        reports.emplace_back(order.clientOrderId, systemOrderId, order.instrument,
                             order.side, tradePrice, tradeQty, aggStatus,
                             Utils::getCurrentTransactionTime());

        // Generate Execution Report for the Passive (Resting) Order
        ExecStatus passStatus = (restingOrder.quantity == 0) ? ExecStatus::Fill : ExecStatus::PFill;
        reports.emplace_back(restingOrder.clientOrderId, restingOrder.systemOrderId, restingOrder.instrument,
                             restingOrder.side, tradePrice, tradeQty, passStatus,
                             Utils::getCurrentTransactionTime());

        // Clean up the order book if the resting order is fully filled
        if (restingOrder.quantity == 0)
        {
            restingOrders.pop();
            // If there are no more orders at this price level, erase the price level from the map
            if (restingOrders.empty())
            {
                sellSide.erase(bestSellIt);
            }
        }
    }

    // Determine what to do with any remaining quantity of the incoming order
    if (order.quantity == initialQty)
    {
        // The order matched absolutely nothing. It is a completely new passive order.
        reports.emplace_back(order.clientOrderId, systemOrderId, order.instrument,
                             order.side, order.price, order.quantity, ExecStatus::New,
                             Utils::getCurrentTransactionTime());

        order.systemOrderId = systemOrderId;
        buySide[order.price].push(order);
    }
    else if (order.quantity > 0)
    {
        // The order partially filled. The remainder goes into the book silently.
        order.systemOrderId = systemOrderId;
        buySide[order.price].push(order);
    }

    return reports;
}

std::vector<ExecutionReportRow> OrderBook::matchSellOrder(Order &order, const std::string &systemOrderId)
{
    std::vector<ExecutionReportRow> reports;
    int initialQty = order.quantity;

    while (order.quantity > 0 && !buySide.empty())
    {
        auto bestBuyIt = buySide.begin(); // std::greater keeps the highest price at the front
        double bestBuyPrice = bestBuyIt->first;

        // If the best available buy price is lower than what we are willing to sell for, stop matching.
        if (bestBuyPrice < order.price)
        {
            break;
        }

        OrderQueue &restingOrders = bestBuyIt->second;
        Order &restingOrder = restingOrders.front();

        int tradeQty = std::min(order.quantity, restingOrder.quantity);
        double tradePrice = bestBuyPrice;

        order.quantity -= tradeQty;
        restingOrder.quantity -= tradeQty;

        ExecStatus aggStatus = (order.quantity == 0) ? ExecStatus::Fill : ExecStatus::PFill;
        reports.emplace_back(order.clientOrderId, systemOrderId, order.instrument,
                             order.side, tradePrice, tradeQty, aggStatus,
                             Utils::getCurrentTransactionTime());

        ExecStatus passStatus = (restingOrder.quantity == 0) ? ExecStatus::Fill : ExecStatus::PFill;
        reports.emplace_back(restingOrder.clientOrderId, restingOrder.systemOrderId, restingOrder.instrument,
                             restingOrder.side, tradePrice, tradeQty, passStatus,
                             Utils::getCurrentTransactionTime());

        if (restingOrder.quantity == 0)
        {
            restingOrders.pop();
            if (restingOrders.empty())
            {
                buySide.erase(bestBuyIt);
            }
        }
    }

    if (order.quantity == initialQty)
    {
        reports.emplace_back(order.clientOrderId, systemOrderId, order.instrument,
                             order.side, order.price, order.quantity, ExecStatus::New,
                             Utils::getCurrentTransactionTime());

        order.systemOrderId = systemOrderId;
        sellSide[order.price].push(order);
    }
    else if (order.quantity > 0)
    {
        order.systemOrderId = systemOrderId;
        sellSide[order.price].push(order);
    }

    return reports;
}