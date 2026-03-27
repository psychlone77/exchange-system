#include "core/FlowerExchange.h"
#include "core/Validator.h"
#include "io/Utils.h"

FlowerExchange::FlowerExchange()
{
    // Initialize the order books for the 5 strictly supported flower types.
    // emplace() is highly efficient as it constructs the OrderBook directly inside the map.
    orderBooks.emplace(Instrument::Rose, OrderBook(Instrument::Rose));
    orderBooks.emplace(Instrument::Lavender, OrderBook(Instrument::Lavender));
    orderBooks.emplace(Instrument::Lotus, OrderBook(Instrument::Lotus));
    orderBooks.emplace(Instrument::Tulip, OrderBook(Instrument::Tulip));
    orderBooks.emplace(Instrument::Orchid, OrderBook(Instrument::Orchid));
}

std::string FlowerExchange::generateOrderId()
{
    // Generates the sequential IDs required by the spec: "ord1", "ord2", "ord3", etc.
    return "ord" + std::to_string(currentOrderId++);
}

std::vector<ExecutionReportRow> FlowerExchange::processOrder(Order &order)
{
    std::string rejectReason;

    // 1. Validate the incoming order
    if (!Validator::isValid(order, rejectReason))
    {
        // If validation fails, we immediately generate a Rejected execution report.
        std::vector<ExecutionReportRow> rejectedReport;

        // According to the sample output on slide 21, rejected orders still receive
        // a system-generated Order ID in the sequence.
        std::string rejectedOrderId = generateOrderId();

        rejectedReport.emplace_back(
            order.clientOrderId,
            rejectedOrderId,
            order.instrument,
            order.side,
            order.price,
            order.quantity,
            ExecStatus::Rejected,
            Utils::getCurrentTransactionTime(), // We will implement this helper next
            rejectReason);

        return rejectedReport;
    }

    // 2. If valid, generate the unique system Order ID for this new order
    std::string systemOrderId = generateOrderId();

    // 3. Route to the correct Order Book
    // We use .at() which is safe here because Validator::isValid already guaranteed
    // that order.instrument is not Instrument::Invalid.
    return orderBooks.at(order.instrument).processOrder(order, systemOrderId);
}