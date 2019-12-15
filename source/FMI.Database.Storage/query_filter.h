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

    const bool uses_index()const { return useIndex; }
    const std::string &get_table_name()const { return tableName; }
    const std::vector<database_column_filter> &get_column_fitlers()const { return columnComparisons; }


    bool test(const table_row &row)const
    {
        bool result = true;

        for (const database_column_filter &filter : columnComparisons)
        {
            const std::string &column = filter.get_column();
            const database_value left = row.get(column);
            const database_value right = filter.get_operand();


            switch (filter.get_operator())
            {
            case database_column_operator::COLUMN_EQUALS:
                result &= left == right; break;
            case database_column_operator::COLUMN_DOES_NOT_EQUAL:
                result &= left != right; break;
            case database_column_operator::COLUMN_IS_GREATER_THAN:
                result &= left > right; break;
            case database_column_operator::COLUMN_IS_GREATER_THAN_OR_EQUAL:
                result &= left >= right; break;
            case database_column_operator::COLUMN_IS_LESS_THAN:
                result &= left < right; break;
            case database_column_operator::COLUMN_IS_LESS_THAN_OR_EQUAL:
                result &= left <= right; break;
            default:
                throw std::runtime_error("attempt to test with unsupported operator");
            }

            if (!result)break;
        }

        return result;
    }

    ~query_filter() {}
};
