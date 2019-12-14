#pragma once
#include "stdafx.h"
#include "database_value.h"
#include "index_key.h"
#include "table_schema.h"
#include <map>
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

    index_key get_primary_key(const table_schema &table)const
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

        index_key key{ keyValues };
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
};


class table_row_builder
{
    std::map<std::string, database_value> values;

public:

    table_row_builder() {}

    table_row_builder(const table_row_builder &other) :
        values(other.values) {}

    table_row_builder(const table_row &copyFrom)
    {
        std::vector<std::string> keys = copyFrom.get_keys();

        for (const std::string &key : keys)
        {
            values[key] = copyFrom.get(key);
        }
    }

    table_row_builder& set(std::string column, database_value value)
    {
        values.emplace(column, value);
        return *this;
    }

    table_row build()const { return { values }; }
};