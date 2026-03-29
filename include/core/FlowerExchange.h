#pragma once

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <thread>
#include "models/Order.h"
#include "OrderBook.h"
#include "models/ExecutionReportRow.h"
#include "interfaces/IOrderReader.h"
#include "interfaces/IExecutionWriter.h"
#include "ThreadSafeQueue.h"

class FlowerExchange
{
private:
    std::shared_ptr<IOrderReader> reader;
    std::shared_ptr<IExecutionWriter> writer;

    // Output queue for execution reports
    ThreadSafeQueue<std::vector<ExecutionReportRow>> outputQueue;

    // Instrument queues mapping from instrument type to queue pointer
    std::map<Instrument, std::shared_ptr<ThreadSafeQueue<Order>>> instrumentQueues;

    // Threads
    std::vector<std::thread> instrumentThreads;
    std::thread writerThread;
    std::thread producerThread;

    int currentOrderId = 1;

    void instrumentWorker(Instrument inst);
    void writerWorker();
    void producerWorker();

public:
    FlowerExchange(std::shared_ptr<IOrderReader> reader, std::shared_ptr<IExecutionWriter> writer);
    ~FlowerExchange() = default;

    void start();
};