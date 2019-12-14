#pragma once
#include <vector>
#include <map>
#include <string>
#include "column_schema.h"

class index_schema
{
    std::vector<std::string> columns;
    bool uniqueConstraint;
public:

    index_schema(const std::vector<std::string> columns, bool uniqueConstraint)
    {
        this->columns = columns;
        this->uniqueConstraint = uniqueConstraint;
    }

    bool constraint_unique()const { return uniqueConstraint; }
};

