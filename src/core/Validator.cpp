#include "core/Validator.h"

bool Validator::isValid(const Order &order, std::string &rejectReason)
{
    // We execute checks sequentially. The first check that fails will
    // populate the rejectReason string and immediately short-circuit (return false).
    if (order.clientOrderId.empty())
    {
        rejectReason = "Missing Client Order ID";
        return false;
    }

    if (!checkInstrument(order, rejectReason))
        return false;
    if (!checkSide(order, rejectReason))
        return false;
    if (!checkPrice(order, rejectReason))
        return false;
    if (!checkQuantity(order, rejectReason))
        return false;

    // If it passes all checks, it's a valid order
    return true;
}

bool Validator::checkInstrument(const Order &order, std::string &reason)
{
    // Check if the parser encountered an unknown flower type
    if (order.instrument == Instrument::Invalid)
    {
        reason = "Invalid instrument"; // Exact string from slide 21
        return false;
    }
    return true;
}

bool Validator::checkSide(const Order &order, std::string &reason)
{
    // Check if the side is anything other than 1 (Buy) or 2 (Sell)
    if (order.side == Side::Unknown)
    {
        reason = "Invalid side"; // Exact string from slide 21
        return false;
    }
    return true;
}

bool Validator::checkPrice(const Order &order, std::string &reason)
{
    // Price must be strictly greater than 0.0
    if (order.price <= 0.0)
    {
        reason = "Invalid price"; // Exact string from slide 21
        return false;
    }
    return true;
}

bool Validator::checkQuantity(const Order &order, std::string &reason)
{
    // Quantity limits: Min 10, Max 1000
    // Quantity multiple: Must be a multiple of 10
    if (order.quantity < 10 || order.quantity > 1000 || order.quantity % 10 != 0)
    {
        reason = "Invalid size"; // Exact string from slide 21
        return false;
    }
    return true;
}