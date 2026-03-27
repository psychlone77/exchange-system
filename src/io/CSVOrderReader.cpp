#include "io/CSVOrderReader.h"
#include "vendor/csv.h"
#include <iostream>
#include <stdexcept>

// Internal helper function isolated to this translation unit
static Instrument parseInstrument(const std::string &instStr)
{
    if (instStr == "Rose")
        return Instrument::Rose;
    // Note: Handling a potential typo from the assignment examples ("Lavendar" vs "Lavender")
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

std::vector<Order> CSVOrderReader::readOrders()
{
    std::vector<Order> orders;

    // Pre-allocate memory if you expect large files, though fast-cpp-csv-parser
    // is fast enough that dynamic reallocation usually isn't the bottleneck.
    orders.reserve(100000);

    try
    {
        // Initialize the parser for 5 columns.
        // We configure it to trim whitespaces and handle potential quotes.
        io::CSVReader<5, io::trim_chars<' ', '\t'>, io::double_quote_escape<',', '\"'>> csvReader(filePath);

        // This ensures the columns match the exact names provided in the spec.
        // It also handles out-of-order columns automatically!
        csvReader.read_header(io::ignore_extra_column, "Cl. Ord.ID", "Instrument", "Side", "Quantity", "Price");

        std::string clOrdId;
        std::string instrumentStr;
        int sideInt;
        int quantity;
        double price;

        // read_row parses directly into our native variables. No intermediate string copying for the ints/doubles!
        while (csvReader.read_row(clOrdId, instrumentStr, sideInt, quantity, price))
        {

            Instrument inst = parseInstrument(instrumentStr);

            // The spec defines Side as an Int (1: Buy, 2: Sell)
            Side side = Side::Unknown;
            if (sideInt == 1)
            {
                side = Side::Buy;
            }
            else if (sideInt == 2)
            {
                side = Side::Sell;
            }

            // Emplace back constructs the Order object directly inside the vector's memory space,
            // using std::move to prevent copying the clOrdId string.
            orders.emplace_back(std::move(clOrdId), inst, side, quantity, price);
        }
    }
    catch (const io::error::can_not_open_file &e)
    {
        std::cerr << "CRITICAL ERROR: Could not open the orders file at " << filePath << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "CRITICAL ERROR: CSV Parsing failed - " << e.what() << "\n";
    }

    return orders;
}