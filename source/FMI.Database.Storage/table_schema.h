#pragma once
#include <string>
#include <set>
#include <map>
#include <vector>
#include "column_schema.h"


class table_schema
{
    std::string name;
    std::map<std::string, column_schema> columns;
    std::vector<std::string> primaryKey;

public:
    table_schema() :table_schema("_") {}
    table_schema(const std::string &name) :name(name) {}
    table_schema(const table_schema &other) :
        name(other.name), columns(other.columns), primaryKey(other.primaryKey) {}

    table_schema(
        const std::string &name,
        const std::vector<column_schema> &columns,
        const std::vector<std::string> &primaryKey
    )
    {
        this->name = name;
        this->columns = map_columns(columns);
        this->primaryKey = assert_primary_key(this->columns, primaryKey);
    }

    table_schema(
        const std::string &name,
        const std::map<std::string, column_schema> columns,
        const std::vector<std::string> primaryKey
    )
    {
        assert_primary_key(columns, primaryKey);

        this->name = name;
        this->columns = columns;
        this->primaryKey = primaryKey;
    }

    column_schema get_column(const std::string &name)const
    {
        return columns.at(name);
    }

    int column_count()const { return columns.size(); }

    std::vector<column_schema> get_primary_key()const
    {
        std::vector<column_schema> key;
        for (const std::string& name : this->primaryKey)
        {
            key.push_back(columns.at(name));
        }
        return key;
    }

    bool has_column(const std::string &name)const
    {
        return columns.find(name) != columns.end();
    }

    std::vector<column_schema> get_columns()const
    {
        std::vector<column_schema> columns;

        for (const auto &x : this->columns)
        {
            columns.push_back(x.second);
        }

        return columns;
    }

private:

    std::map<std::string, column_schema> map_columns(const std::vector<column_schema> &columns)
    {
        std::map<std::string, column_schema> mapped;
        for (const column_schema &column : columns)
        {
            mapped.emplace(column.get_name(), column);
        }

        if (mapped.size() != columns.size())
            throw std::runtime_error("can't have two columns with the same name!");

        return mapped;
    }

    const std::vector<std::string> assert_primary_key(
        const std::map<std::string, column_schema> &columns,
        const std::vector<std::string> &primaryKey
    )
    {
        if (!check_primary_key(columns, primaryKey))
            throw std::runtime_error("attempted to use an empty or invalid primary key for table");

        return primaryKey;
    }



    bool check_primary_key(
        const std::map<std::string, column_schema> &columns,
        const std::vector<std::string> &primaryKey
    )
    {
        using namespace std;
        if (primaryKey.size() > columns.size())return false;

        set<string> keys;

        for (const string &keyColumn : primaryKey)
        {
            keys.insert(keyColumn);

            auto found = columns.find(keyColumn);
            if (found == columns.end())
                return false;
        }

        if (keys.size() != primaryKey.size())
            return false;

        return true;
    }
};