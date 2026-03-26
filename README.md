# Flower Exchange - C++ Implementation

flower_exchange/
├── CMakeLists.txt
├── data/
│   ├── orders.csv
│   └── execution_rep.csv
├── include/
│   ├── models/          # Data structures, structs, and enums (DTOs)
│   │   ├── Order.h
│   │   └── ExecutionReport.h
│   ├── interfaces/      # Pure abstract classes (Contracts)
│   │   ├── IOrderReader.h
│   │   └── IExecutionWriter.h
│   ├── core/            # The brain of the matching engine
│   │   ├── Validator.h
│   │   ├── OrderBook.h
│   │   └── ExchangeApp.h
│   └── io/              # Concrete implementations for file handling
│   │   ├── CSVOrderReader.h
│   │   └── CSVExecutionWriter.h
├── src/
│   ├── main.cpp         # The entry point that wires everything together
│   ├── core/            # Implementations of the core engine
│   │   ├── Validator.cpp
│   │   ├── OrderBook.cpp
│   │   └── ExchangeApp.cpp
│   └── io/              # Implementations of the I/O handlers
│       ├── CSVOrderReader.cpp
│       └── CSVExecutionWriter.cpp
└── build/
