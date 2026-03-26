#pragma once

#include <string>
#include "Instrument.h"
#include "Side.h"

enum class ExecStatus
{
    New = 0,
    Rejected = 1,
    Fill = 2,
    PFill = 3,
};

struct ExecutionReportRow
{
    std::string clientOrderId;

    std::string orderId;

    Instrument instrument;

    Side side;

    double price;

    int quantity;

    ExecStatus status;

    std::string reason;

    std::string transactionTime;

    ExecutionReportRow(std::string clOrdId, std::string ordId, Instrument inst,
                       Side s, double pr, int qty, ExecStatus stat,
                       std::string transTime, std::string rsn = "")
        : clientOrderId(std::move(clOrdId)),
          orderId(std::move(ordId)),
          instrument(inst),
          side(s),
          price(pr),
          quantity(qty),
          status(stat),
          reason(std::move(rsn)),
          transactionTime(std::move(transTime)) {}

    ExecutionReportRow() : price(0.0), quantity(0), status(ExecStatus::New) {}
};