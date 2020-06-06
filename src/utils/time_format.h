//
// Created by dvl on 10/14/19.
//

#ifndef MODSECURITY_TIME_HPP
#define MODSECURITY_TIME_HPP

#include <string>
#include <time.h>

const std::size_t default_time_str_size = 300;

template <std::size_t c_str_length = default_time_str_size>
std::string get_formatted_time_string(const char* format, time_t* time){
    char time_str[c_str_length];
    struct tm time_info;

    localtime_r(time, &time_info);
    auto ret = strftime(time_str, c_str_length, format, &time_info);

    return std::string{
        ret ? time_str : ""
    };
}


template <std::size_t c_str_length = default_time_str_size>
std::string get_formatted_time_string_now(const char* format){
    time_t time_obj;
    time(&time_obj);

    return get_formatted_time_string<c_str_length>(format, &time_obj);
}

#endif // MODSECURITY_TIME_HPP
