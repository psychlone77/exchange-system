#pragma once

#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace Utils
{

    // Helper to get the current transaction time strictly formatted as YYYYMMDD-HHMMSS.sss
    inline std::string getCurrentTransactionTime()
    {
        // Grab the current system time
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        // Extract the millisecond component
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        std::stringstream ss;

        // Format the date and time: YYYYMMDD-HHMMSS
        ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d-%H%M%S");

        // Append the milliseconds: .sss
        ss << '.' << std::setfill('0') << std::setw(3) << ms.count();

        return ss.str();
    }

}