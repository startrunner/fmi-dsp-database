#pragma once
#include <string>
#include <set>
#include <map>
#include <vector>
#include "column_schema.h"


class table_schema
{
    std::string name;
    std::map<std::string, column_schema> specifiedColumns;
    std::vector<std::string> primaryKey;

public:
    table_schema() :table_schema("_") {}
    table_schema(const std::string &name) :name(name) {}
    table_schema(const table_schema &other) :
        name(other.name), specifiedColumns(other.specifiedColumns), primaryKey(other.primaryKey)
    {}

    table_schema(
        const std::string &name,
        const std::vector<column_schema> &specifiedColumns,
        const std::vector<std::string> &primaryKey
    )
    {
        this->name = name;
        this->specifiedColumns = map_columns(specifiedColumns);
        this->primaryKey = assert_primary_key(this->specifiedColumns, primaryKey);
    }

    table_schema(
        const std::string &name,
        const std::map<std::string, column_schema> specifiedColumns,
        const std::vector<std::string> primaryKey
    )
    {
        assert_primary_key(specifiedColumns, primaryKey);

        this->name = name;
        this->specifiedColumns = specifiedColumns;
        this->primaryKey = primaryKey;
    }

    column_schema get_column(const std::string &name)const
    {
        return specifiedColumns.at(name);
    }

    size_t column_count()const { return specifiedColumns.size(); }

    std::string get_name()const { return name; }

    std::vector<column_schema> get_primary_key()const
    {
        std::vector<column_schema> key;
        for (const std::string& name : this->primaryKey)
        {
            key.push_back(specifiedColumns.at(name));
        }
        return key;
    }

    bool has_column(const std::string &name)const
    {
        return specifiedColumns.find(name) != specifiedColumns.end();
    }

    std::vector<column_schema> get_columns()const
    {
        std::vector<column_schema> specifiedColumns;

        for (const auto &x : this->specifiedColumns)
        {
            specifiedColumns.push_back(x.second);
        }

        return specifiedColumns;
    }

    void dump(std::ostream &out)const
    {
        using namespace std;
        out << "Table schema name: " << name << endl;
        out << "Table schema columns: ";
        int i = 0;
        for (const auto &column : this->specifiedColumns)
        {
            column.second.dump(out);
            if (i + 1 < this->specifiedColumns.size())out << ", ";
            i++;
        }
        out << endl;

        out << "Table schema primary key: ";
        i = 0;
        for (const std::string &pkColumn : this->primaryKey)
        {
            out << pkColumn;
            if (i + 1 < this->primaryKey.size())out << ", ";
            i++;
        }
        out << endl;

    }

private:

    std::map<std::string, column_schema> map_columns(const std::vector<column_schema> &specifiedColumns)
    {
        std::map<std::string, column_schema> mapped;
        for (const column_schema &column : specifiedColumns)
        {
            mapped.emplace(column.get_name(), column);
        }

        if (mapped.size() != specifiedColumns.size())
            throw std::runtime_error("can't have two columns with the same name!");

        return mapped;
    }

    const std::vector<std::string> assert_primary_key(
        const std::map<std::string, column_schema> &specifiedColumns,
        const std::vector<std::string> &primaryKey
    )
    {
        if (!check_primary_key(specifiedColumns, primaryKey))
            throw std::runtime_error("attempted to use an empty or invalid primary key for table");

        return primaryKey;
    }



    bool check_primary_key(
        const std::map<std::string, column_schema> &specifiedColumns,
        const std::vector<std::string> &primaryKey
    )
    {
        using namespace std;
        if (primaryKey.size() > specifiedColumns.size())return false;

        set<string> keys;

        for (const string &keyColumn : primaryKey)
        {
            keys.insert(keyColumn);

            auto found = specifiedColumns.find(keyColumn);
            if (found == specifiedColumns.end())
                return false;
        }

        if (keys.size() != primaryKey.size())
            return false;

        return true;
    }


};