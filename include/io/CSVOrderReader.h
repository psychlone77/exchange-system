#pragma once

#include <string>
#include <memory>
#include "interfaces/IOrderReader.h"
#include "vendor/csv.h"

class CSVOrderReader : public IOrderReader
{
private:
    std::string filePath;
    using ParserType = io::CSVReader<5, io::trim_chars<' ', '\t'>, io::double_quote_escape<',', '\"'>>;
    std::unique_ptr<ParserType> csvParser;

public:
    explicit CSVOrderReader(std::string path);

    bool readNextOrder(Order &order) override;
};