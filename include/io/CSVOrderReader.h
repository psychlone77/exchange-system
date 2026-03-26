#pragma once

#include <string>
#include "interfaces/IOrderReader.h"

class CSVOrderReader : public IOrderReader
{
private:
    std::string filePath;

public:
    explicit CSVOrderReader(std::string path) : filePath(std::move(path)) {}

    std::vector<Order> readOrders() override;
};