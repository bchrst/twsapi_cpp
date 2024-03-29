﻿/* Copyright (C) 2019 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */
#pragma once
#ifndef TWS_API_SAMPLES_TESTCPPCLIENT_UTILS_H
#define TWS_API_SAMPLES_TESTCPPCLIENT_UTILS_H

#include <vector>
#include <cstdint>
#include <string>
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <chrono>
#include <iostream>

class Utils {

public:
    static inline bool is_base64(std::uint8_t c);
    static std::vector<std::uint8_t> base64_decode(std::string const&);
    static std::string formatDoubleString(std::string const&);
    static std::string get_timestamp(std::chrono::time_point<std::chrono::system_clock> base_time, uint64_t delay_in_secs);
    static std::chrono::time_point<std::chrono::system_clock> stream_ticks(std::string last_tick);
};

#endif
