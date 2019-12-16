#pragma once
#include "stdafx.h"
#include <variant>
#include "database_type.h"
#include "dbtime.h"
#include "uuid.h"

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
        if (this->type != other.type)
        {
            throw std::runtime_error("comparison of incompatible types");
        }

        return value < other.value;
    }

    bool operator > (const database_value &other)const { return other < *this; }

    bool operator <= (const database_value &other)const { return *this < other || *this == other; }

    bool operator >= (const database_value &other)const { return other < *this || other == *this; }

    bool operator == (const database_value &other)const { return type == other.type && value == other.value; }

    bool operator != (const database_value &other)const { return !(*this == other); }

    friend std::ostream& operator << (std::ostream& out, const database_value &toPrint);

    std::string to_string()const
    {
        std::ostringstream stream;
        stream << *this;
        return stream.str();
    }
};
