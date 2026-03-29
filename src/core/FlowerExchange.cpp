#include "core/FlowerExchange.h"
#include "core/Validator.h"
#include "io/Utils.h"

FlowerExchange::FlowerExchange(std::shared_ptr<IOrderReader> reader, std::shared_ptr<IExecutionWriter> writer)
    : reader(std::move(reader)), writer(std::move(writer))
{
    instrumentQueues[Instrument::Rose] = std::make_shared<ThreadSafeQueue<Order>>();
    instrumentQueues[Instrument::Lavender] = std::make_shared<ThreadSafeQueue<Order>>();
    instrumentQueues[Instrument::Lotus] = std::make_shared<ThreadSafeQueue<Order>>();
    instrumentQueues[Instrument::Tulip] = std::make_shared<ThreadSafeQueue<Order>>();
    instrumentQueues[Instrument::Orchid] = std::make_shared<ThreadSafeQueue<Order>>();
}

void FlowerExchange::start()
{
    writerThread = std::thread(&FlowerExchange::writerWorker, this);

    instrumentThreads.emplace_back(&FlowerExchange::instrumentWorker, this, Instrument::Rose);
    instrumentThreads.emplace_back(&FlowerExchange::instrumentWorker, this, Instrument::Lavender);
    instrumentThreads.emplace_back(&FlowerExchange::instrumentWorker, this, Instrument::Lotus);
    instrumentThreads.emplace_back(&FlowerExchange::instrumentWorker, this, Instrument::Tulip);
    instrumentThreads.emplace_back(&FlowerExchange::instrumentWorker, this, Instrument::Orchid);

    producerThread = std::thread(&FlowerExchange::producerWorker, this);

    if (producerThread.joinable())
    {
        producerThread.join();
    }

    for (auto &pair : instrumentQueues)
    {
        pair.second->shutdown();
    }

    for (auto &th : instrumentThreads)
    {
        if (th.joinable())
            th.join();
    }

    outputQueue.shutdown();

    if (writerThread.joinable())
    {
        writerThread.join();
    }
}

void FlowerExchange::producerWorker()
{
    Order order;
    while (reader->readNextOrder(order))
    {
        std::string rejectReason;
        if (!Validator::isValid(order, rejectReason))
        {
            std::vector<ExecutionReportRow> rejectedReport;
            std::string rejectedOrderId = "ord" + std::to_string(currentOrderId++);

            rejectedReport.emplace_back(
                order.clientOrderId,
                rejectedOrderId,
                order.instrument,
                order.side,
                order.price,
                order.quantity,
                ExecStatus::Rejected,
                Utils::getCurrentTransactionTime(),
                rejectReason);

            outputQueue.push(std::move(rejectedReport));
        }
        else
        {
            order.systemOrderId = "ord" + std::to_string(currentOrderId++);
            instrumentQueues[order.instrument]->push(order);
        }
    }
}

void FlowerExchange::instrumentWorker(Instrument inst)
{
    OrderBook orderBook(inst);
    Order order;
    while (instrumentQueues[inst]->pop(order))
    {
        std::vector<ExecutionReportRow> reports = orderBook.processOrder(order, order.systemOrderId);
        outputQueue.push(std::move(reports));
    }
}

void FlowerExchange::writerWorker()
{
    std::vector<ExecutionReportRow> allExecutions;
    std::vector<ExecutionReportRow> batch;

    while (outputQueue.pop(batch))
    {
        allExecutions.insert(allExecutions.end(), batch.begin(), batch.end());
    }

    if (!allExecutions.empty() && writer)
    {
        writer->writeExecutions(allExecutions);
    }
}