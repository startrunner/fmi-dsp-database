#pragma once
#include <vector>
#include <map>
#include <tuple>
#include <string>
#include "database_query.h"
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

    index_schema(
        const std::vector<std::string> &columns,
        const std::vector<database_column_operator> &operators
    )
    {
        if (columns.empty())
            throw std::runtime_error("could not create index schema without columns!");

        this->columns = columns;
    }

};

