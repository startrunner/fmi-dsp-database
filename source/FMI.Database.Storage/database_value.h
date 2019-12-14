#pragma once
#include "stdafx.h"
#include <vector>
#include <map>
#include <cinttypes>
#include <cstddef>
#include <chrono>
#include <variant>
#include <string>
#include <stdexcept>
#include "boost_uuid.h"
#include "database_type.h"
#include "database_value.h"
#include "table_schema.h"

struct dbtime
{
private:
    time_t time;

public:
    dbtime() :dbtime(time_t{}) {}
    dbtime(const dbtime &other) :dbtime(other.time) {}
    dbtime(time_t time) :time(time) {}

    dbtime& operator = (const dbtime &other)
    {
        time = other.time;
        return *this;
    }

    bool operator < (dbtime other)const { return time < other.time; }
    bool operator > (dbtime other)const { return time > other.time; }
    bool operator <= (dbtime other)const { return time <= other.time; }
    bool operator >= (dbtime other)const { return time >= other.time; }
    bool operator == (dbtime other)const { return time == other.time; }
    bool operator != (dbtime other)const { return time != other.time; }

    static dbtime now();
};

dbtime dbtime::now()
{
    time_t inner = ::time(nullptr);
    dbtime time{ inner };
    return time;
}

class database_value
{
    database_type type;

    using value_t = std::variant<
        int32_t,
        int64_t,
        double,
        bool,
        dbtime,
        lib::uuid,
        std::string
    >;
    value_t value;



    //value_t value;

    //todo: use std::variant<int32, int64, ...>
    //struct//union
    //{

    //    int32_t int32;
    //    int64_t int64;
    //    double doubleValue;
    //    bool boolean;
    //    dbtime time;
    //    lib::uuid uuid;
    //    std::string string;
    //} value;

public:

    database_value() {}

    //database_value() :database_value("_") {}

    database_value(const database_value &other)
    {
        this->type = other.type;
        value = other.value;
    }

    database_value(dbtime time) :
        value(value), type(database_type::TIME) {}

    database_value(bool value) :
        value(value), type(database_type::BOOLEAN) {}

    database_value(int32_t value) :
        value(value), type(database_type::INT32) {}

    database_value(int64_t value) :
        value(value), type(database_type::INT64) {}

    database_value(lib::uuid value) :
        value(value), type(database_type::UUID) {}

    database_value(std::string value) :
        value(value), type(database_type::TEXT) {}

    database_value(const char *text) :
        value(std::string{ text }), type(database_type::TEXT) {}

    database_value(double value) :
        value(value), type(database_type::DOUBLE) {}

    database_type get_type()const { return type; }

    bool operator < (const database_value &other)const
    {
        if (type != other.type)
            throw std::runtime_error("comparison of incompatible types...");

        bool result = value < other.value;

        return result;

    }
};

