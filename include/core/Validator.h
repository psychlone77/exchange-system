#pragma once

#include <string>
#include "models/Order.h"

class Validator
{
public:
    /*
     * Validates an order based on predefined criteria.
     * @param order The order to validate.
     * @param rejectReason A reference to a string that will hold the reason for rejection if the order is invalid.
     * @return True if the order is valid, false otherwise.
     */
    static bool isValid(const Order &order, std::string &rejectReason);

private:
    static bool checkInstrument(const Order &order, std::string &reason);
    static bool checkSide(const Order &order, std::string &reason);
    static bool checkPrice(const Order &order, std::string &reason);
    static bool checkQuantity(const Order &order, std::string &reason);
};