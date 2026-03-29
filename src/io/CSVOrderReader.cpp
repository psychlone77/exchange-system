#include "io/CSVOrderReader.h"
#include <iostream>
#include <stdexcept>

// Internal helper function isolated to this translation unit
static Instrument parseInstrument(const std::string &instStr)
{
    if (instStr == "Rose")
        return Instrument::Rose;
    if (instStr == "Lavender" || instStr == "Lavendar")
        return Instrument::Lavender;
    if (instStr == "Lotus")
        return Instrument::Lotus;
    if (instStr == "Tulip")
        return Instrument::Tulip;
    if (instStr == "Orchid")
        return Instrument::Orchid;
    return Instrument::Invalid;
}

CSVOrderReader::CSVOrderReader(std::string path) : filePath(std::move(path))
{
    try
    {
        csvParser = std::make_unique<ParserType>(filePath);
        csvParser->read_header(io::ignore_extra_column, "Cl. Ord.ID", "Instrument", "Side", "Quantity", "Price");
    }
    catch (const io::error::can_not_open_file &e)
    {
        std::cerr << "CRITICAL ERROR: Could not open the orders file at " << filePath << "\n";
        csvParser.reset();
    }
    catch (const std::exception &e)
    {
        std::cerr << "CRITICAL ERROR: CSV Header Parsing failed - " << e.what() << "\n";
        csvParser.reset();
    }
}

bool CSVOrderReader::readNextOrder(Order &order)
{
    if (!csvParser)
        return false;

    std::string clOrdId;
    std::string instrumentStr;
    int sideInt;
    int quantity;
    double price;

    try
    {
        if (csvParser->read_row(clOrdId, instrumentStr, sideInt, quantity, price))
        {
            Instrument inst = parseInstrument(instrumentStr);
            Side side = Side::Unknown;
            if (sideInt == 1)
            {
                side = Side::Buy;
            }
            else if (sideInt == 2)
            {
                side = Side::Sell;
            }

            order.clientOrderId = std::move(clOrdId);
            order.instrument = inst;
            order.side = side;
            order.quantity = quantity;
            order.price = price;

            return true;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "CRITICAL ERROR: CSV Row Parsing failed - " << e.what() << "\n";
    }

    return false;
}