#pragma once
#include <string>
#include <iostream>
#include "database_value.h"
#include "table_row.h"

enum class database_column_operator
{
    COLUMN_EQUALS,
    COLUMN_IS_LESS_THAN,
    COLUMN_IS_LESS_THAN_OR_EQUAL,
    COLUMN_IS_GREATER_THAN,
    COLUMN_IS_GREATER_THAN_OR_EQUAL,
    COLUMN_DOES_NOT_EQUAL,
};

class database_column_filter
{
    std::string column;
    database_column_operator theOperator;
    database_value operand;

public:

public:
    const std::string& get_column()const { return column; }
    database_column_operator get_operator()const { return theOperator; }
    database_value get_operand()const { return operand; }


    database_column_filter() :
        database_column_filter("_", database_column_operator::COLUMN_EQUALS, 0)
    {}

    database_column_filter(
        std::string column,
        database_column_operator theOperator,
        database_value operand
    )
    {
        this->column = column;
        this->theOperator = theOperator;
        this->operand = operand;
    }
};

class query_filter
{
    bool useIndex;
    std::string tableName;
    std::vector<database_column_filter> columnComparisons;

public:

    query_filter() {}

    query_filter(
        const std::string &tableName,
        const std::vector<database_column_filter> &columnComparisons,
        bool useIndex
    )
    {
        this->tableName = tableName;
        this->useIndex = useIndex;
        this->columnComparisons = columnComparisons;
    }

    query_filter(const query_filter &other) { *this = other; }

    query_filter& operator = (const query_filter &other)
    {
        if (this == &other)
            return *this;

        useIndex = other.useIndex;
        tableName = other.tableName;
        columnComparisons = other.columnComparisons;

        return *this;
    }

    const bool uses_index()const { return useIndex; }
    const std::string &get_table_name()const { return tableName; }
    const std::vector<database_column_filter> &get_column_fitlers()const { return columnComparisons; }


    bool test(const table_row &row)const;

    ~query_filter() {}
};
