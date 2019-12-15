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
    std::vector<std::string> columns;
public:

    const std::vector<std::string>& get_columns()const
    {
        return this->columns;
    }

    index_schema() {}

    index_schema(const std::vector<std::string> &columns)
    {
        if (columns.empty())
            throw std::runtime_error("could not create index schema without columns!");

        this->columns = columns;
    }

    index_schema normalize()const
    {
        using namespace std;

        vector<string> columns = this->columns;
        sort(columns.begin(), columns.end());

        return index_schema{ columns };
    }

    bool operator == (const index_schema &other)const { return columns == other.columns; }
    bool operator != (const index_schema &other)const { return columns != other.columns; }
    bool operator <  (const index_schema &other)const { return columns < other.columns; }
    bool operator >  (const index_schema &other)const { return columns > other.columns; }
    bool operator <= (const index_schema &other)const { return columns <= other.columns; }
    bool operator >= (const index_schema &other)const { return columns >= other.columns; }

    void dump(std::ostream &out)const
    {
        out << "Index(";
        for (int i = 0; i < columns.size(); i++)
        {
            out << columns[i];
            if (i + 1 < columns.size())out << ", ";
        }
        out << ')';
    }


};

