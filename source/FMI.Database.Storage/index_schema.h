#pragma once
#include <vector>
#include <map>
#include <tuple>
#include <string>
#include <algorithm>
#include "query_filter.h"
#include "column_schema.h"

class index_schema
{
    std::vector<std::string> specifiedColumns;
public:

    const std::vector<std::string>& get_columns()const
    {
        return this->specifiedColumns;
    }

    index_schema() {}

    index_schema(const std::vector<std::string> &specifiedColumns)
    {
        if (specifiedColumns.empty())
            throw std::runtime_error("could not create index schema without columns!");

        this->specifiedColumns = specifiedColumns;
    }

    index_schema normalize()const
    {
        using namespace std;

        vector<string> specifiedColumns = this->specifiedColumns;
        sort(specifiedColumns.begin(), specifiedColumns.end());

        return index_schema{ specifiedColumns };
    }

    bool operator == (const index_schema &other)const { return specifiedColumns == other.specifiedColumns; }
    bool operator != (const index_schema &other)const { return specifiedColumns != other.specifiedColumns; }
    bool operator <  (const index_schema &other)const { return specifiedColumns < other.specifiedColumns; }
    bool operator >  (const index_schema &other)const { return specifiedColumns > other.specifiedColumns; }
    bool operator <= (const index_schema &other)const { return specifiedColumns <= other.specifiedColumns; }
    bool operator >= (const index_schema &other)const { return specifiedColumns >= other.specifiedColumns; }

    void dump(std::ostream &out)const
    {
        out << "Index(";
        for (int i = 0; i < specifiedColumns.size(); i++)
        {
            out << specifiedColumns[i];
            if (i + 1 < specifiedColumns.size())out << ", ";
        }
        out << ')';
    }


};

