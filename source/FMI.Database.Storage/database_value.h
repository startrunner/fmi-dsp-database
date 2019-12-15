#pragma once
#include "stdafx.h"
#include <vector>
#include <map>
#include <cinttypes>
#include <cstddef>
#include <locale>
#include <chrono>
#include <variant>
#include <iostream>
#include <string>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <ctime>
#include <chrono>
#include <stdexcept>
#include "boost_uuid.h"
#include "database_type.h"
#include "database_value.h"
#include "table_schema.h"
//#include <boost/date_time/gregorian/conversion.hpp>
//#include <boost/date_time/posix_time/conversion.hpp>


struct dbtime
{
private:
    time_t time;

public:
    dbtime() {}
    dbtime(const dbtime &other) 
    {
        this->time = other.time;
    }
    dbtime(time_t time) :time(time) {}

    dbtime& operator = (const dbtime &other)
    {
        if (this == &other)
            return *this;

        this->time = other.time;
        return *this;
    }

    /*dbtime& operator = (const dbtime &other)
    {
        time = other.time;
        return *this;
    }*/

    bool operator < (dbtime other)const { return time < other.time; }
    bool operator > (dbtime other)const { return time > other.time; }
    bool operator <= (dbtime other)const { return time <= other.time; }
    bool operator >= (dbtime other)const { return time >= other.time; }
    bool operator == (dbtime other)const { return time == other.time; }
    bool operator != (dbtime other)const { return time != other.time; }

    operator time_t () const { return time; }

    static dbtime now();

    friend std::ostream& operator << (std::ostream& out, const dbtime &dbTime)
    {
        return out << dbTime.to_string();
    }

    std::string to_string() const
    {
        //fuck this C++ shit
        using namespace std;
        time_t t = this->time;

        //tm *aTime = localtime(&t);
        tm aTime; localtime_s(&aTime, &t);

        int day = aTime.tm_mday;
        int month = aTime.tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
        int year = aTime.tm_year + 1900;

        ostringstream stream;
        stream
            << day << "."
            << month << "."
            << year << " "
            << aTime.tm_hour << ":"
            << aTime.tm_min << ":"
            << aTime.tm_sec;

        string result = stream.str();
        return result;
    }
};

class database_value
{
    //bool reverseOrder = false;
    database_type type;

    using value_t = std::variant<
        dbtime,
        int32_t,
        int64_t,
        double,
        bool,
        lib::uuid,
        std::string
    >;
    value_t value;

public:

    static database_value min_value()
    {
        database_value value;
        value.type = database_type::MIN_VALUE;
        value.value = INT_MIN;
        return value;
    }

    static database_value max_value()
    {
        database_value value;
        value.type = database_type::MAX_VALUE;
        value.value = INT_MAX;
        return value;
    }

    database_value& operator = (const database_value &other)
    {
        this->value = other.value;
        this->type = other.type;
        return *this;
    }

    database_value() {}

    database_value(const database_value &other)
    {
        this->type = other.type;
        value = other.value;
        ;
        //this->reverseOrder = other.reverseOrder;
    }

    database_value(dbtime value) :
        value(value), type(database_type::TIME)
    {}

    database_value(bool value) :
        value(value), type(database_type::BOOLEAN)
    {}

    database_value(int32_t value) :
        value(value), type(database_type::INT32)
    {}

    database_value(int64_t value) :
        value(value), type(database_type::INT64)
    {}

    database_value(lib::uuid value) :
        value(value), type(database_type::UUID)
    {}

    database_value(std::string value) :
        value(value), type(database_type::TEXT)
    {}

    database_value(const char *text) :
        value(std::string{ text }), type(database_type::TEXT)
    {}

    database_value(double value) :
        value(value), type(database_type::DOUBLE)
    {}


    database_type get_type()const { return type; }

    bool operator < (const database_value &other)const
    {
        if (this->type == database_type::MIN_VALUE)return true;
        if (other.type == database_type::MIN_VALUE)return false;
        if (this->type == database_type::MAX_VALUE)return false;
        if (other.type == database_type::MAX_VALUE)return true;

        if (this->type != other.type)
        {
            throw std::runtime_error("comparison of incompatible types");
        }

        return value < other.value;
    }

    bool operator > (const database_value &other)const
    {
        return other < *this;
    }

    bool operator <= (const database_value &other)const
    {
        return *this < other || *this == other;
    }

    bool operator >= (const database_value &other)const
    {
        return other < *this || other == *this;
    }

    bool operator == (const database_value &other)const
    {
        return type == other.type && value == other.value;
    }

    bool operator != (const database_value &other)const
    {
        return !(*this == other);
    }

    friend std::ostream& operator << (std::ostream& out, const database_value &toPrint)
    {
        using namespace std;

        value_t value = toPrint.value;
        switch (toPrint.type)
        {
        case database_type::BOOLEAN:
            out << get<bool>(value) ? "true" : "false";
            break;
        case database_type::DOUBLE:
            out << get<double>(value);
            break;
        case database_type::INT32:
            out << get<int32_t>(value);
            break;
        case database_type::INT64:
            out << get<int64_t>(value);
            break;
        case database_type::TEXT:
            out << get<string>(value);
            break;
        case database_type::TIME:
            out << get<dbtime>(value);
            break;
        case database_type::UUID:
            out << get<boost::uuids::uuid>(value);
            break;

        default:
            break;
        }
        return out;
    }
private:
    void assert_comparison(const database_value &other)const
    {
        if (type != other.type)
            throw std::runtime_error("comparison of incompatible types...");

    }
};
