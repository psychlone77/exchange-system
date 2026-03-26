#pragma once

#include <vector>
#include "models/Order.h"

class IOrderReader
{
public:
    virtual ~IOrderReader() = default;

    virtual std::vector<Order> readOrders() = 0;
};