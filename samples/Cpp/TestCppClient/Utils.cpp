﻿/* Copyright (C) 2019 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */
#include "StdAfx.h"

#include "Utils.h"
#include <iostream>
#include <platformspecific.h>

static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

inline bool Utils::is_base64(std::uint8_t c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::vector<std::uint8_t> Utils::base64_decode(std::string const& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
   int in_ = 0;
    std::uint8_t char_array_4[4], char_array_3[3];
    std::vector<std::uint8_t> ret;

    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret.push_back(char_array_3[i]);
            i = 0;
        }
    }

    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;

        for (j = 0; j <4; j++)
        char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
    }

    return ret;
}

std::string Utils::formatDoubleString(std::string const& str) {
    char buf[25];
    snprintf(buf, sizeof(buf), "%.2f", strtod(str.c_str(), NULL));
    return buf;
}

std::string Utils::get_timestamp(std::chrono::time_point<std::chrono::system_clock> base_time, uint64_t delay_in_secs)
{
    std::chrono::duration<int, std::ratio<1>> delay(delay_in_secs);
    auto in_time_t = std::chrono::system_clock::to_time_t(base_time - delay);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d %X");
    return ss.str();
}


std::chrono::time_point<std::chrono::system_clock> Utils::stream_ticks(std::string last_tick) {
    const char *last_tick_str_cstr = last_tick.c_str();

    int yy, month, dd, hh, mm, ss;
    struct tm tm_start;
    std::time_t next_t;

    sscanf(last_tick_str_cstr, "%4d%2d%2d%2d%2d%2d", &yy, &month, &dd, &hh, &mm, &ss);
    tm_start.tm_year = yy-1900;
    tm_start.tm_mon = month-1;
    tm_start.tm_mday = dd;
    tm_start.tm_hour = hh;
    tm_start.tm_min = mm+1;  // start 1 minute after the last received tick
    tm_start.tm_sec = ss;
    tm_start.tm_isdst = -1;

    next_t = mktime(&tm_start);

    return std::chrono::system_clock::from_time_t(next_t);
}
