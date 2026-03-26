#pragma once

#include <string>
#include "Instrument.h"
#include "Side.h"

struct Order
{
    std::string clientOrderId;

    Instrument instrument;

    Side side;

    int quantity;

    double price;

    Order(std::string clOrdId, Instrument inst, Side s, int qty, double pr)
        : clientOrderId(std::move(clOrdId)),
          instrument(inst), side(s),
          quantity(qty),
          price(pr) {}

    Order() : side(Side::Unknown), instrument(Instrument::Invalid), quantity(0), price(0.0) {}
};