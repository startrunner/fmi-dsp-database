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

    primary_key get_primary_key(const table_schema &table)const
    {
        assert_schema(table);

        std::vector<column_schema> keySchema = table.get_primary_key();

        std::vector<database_value> keyValues;
        keyValues.reserve(keySchema.size());
        for (const column_schema &columnSchema : keySchema)
        {
            database_value value = values.at(columnSchema.get_name());
            keyValues.push_back(value);
        }

        primary_key key{ keyValues };
        return key;

    }

    std::vector<std::string> get_keys()const
    {
        std::vector<std::string> keys;

        for (const auto &entry : values)
        {
            keys.push_back(entry.first);
        }

        return keys;
    }

    database_value get(const std::string &key)const
    {
        return values.at(key);
    }

    /*void set(const std::string column, const database_value &value)
    {
        values[column] = value;
    }
*/
    void assert_schema(const table_schema &table) const
    {
        if (!check_schema(table))
            throw std::runtime_error("row does not comply with schema");
    }

    bool check_schema(const table_schema &tableSchema)const
    {
        if (values.size() > tableSchema.column_count())
            return false;

        for (const auto &entry : values)
        {
            if (!tableSchema.has_column(entry.first))
                return false;

            column_schema columnSchema = tableSchema.get_column(entry.first);

            if (columnSchema.get_type() != entry.second.get_type())
                return false;

        }

        return true;
    }

    void dump(std::ostream &out)const
    {
        //using namespace variant_cout;

        int i = 0;
        for (auto &entry : this->values)
        {
            out << entry.first << ": " << /*streamer{ entry.second }*/entry.second;
            if (i + 1 < values.size())out << ", ";

            i++;
        }
    }

    bool operator < (const table_row &other) const
    {
        return values < other.values;
    }

    bool operator == (const table_row &other) const
    {
        return values == other.values;
    }
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