#pragma once

#include <vector>
#include "models/Order.h"

class IOrderReader
{
public:
    virtual ~IOrderReader() = default;

    virtual bool readNextOrder(Order &order) = 0;
};