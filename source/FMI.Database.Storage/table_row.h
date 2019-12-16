#pragma once
#include "stdafx.h"
#include "database_value.h"
#include "primary_key.h"
#include "table_schema.h"
#include <map>
#include <iostream>
#include <string>

class table_row
{
    std::map<std::string, database_value> values;

public:

    table_row() {}
    table_row(const std::map<std::string, database_value> values)
    {
        this->values = values;
    }

    primary_key get_primary_key(const table_schema &table)const;

    std::vector<std::string> get_keys()const;

    database_value get(const std::string &key)const;
    void assert_schema(const table_schema &table) const;

    bool check_schema(const table_schema &tableSchema)const;

    void dump(std::ostream &out)const;

    bool operator < (const table_row &other) const { return values < other.values; }

    bool operator == (const table_row &other) const { return values == other.values; }
};


class table_row_builder
{
    std::map<std::string, database_value> values;

public:

    table_row_builder() {}

    table_row_builder(const table_row_builder &other) :
        values(other.values)
    {}

    table_row_builder(const table_row &copyFrom)
    {
        std::vector<std::string> keys = copyFrom.get_keys();

        for (const std::string &key : keys)
        {
            values[key] = copyFrom.get(key);
        }
    }

    table_row_builder& set(std::string column, const database_value &value)
    {
        if (value.get_type() == database_type::TIME)
        {
            std::cout << "kur";
            3;
        }
        //values.emplace(column, value);
        values[column] = value;
        return *this;
    }

    table_row build()const
    {
        table_row row{ values };

        return row;
    }
};