#pragma once
#include <string>
#include <iostream>
#include "database_value.h"

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

class database_query
{
    std::string tableName;
    std::vector<database_column_filter> columnComparisons;

public:

    const std::string &get_table_name()const { return tableName; }
    const std::vector<database_column_filter> &get_column_fitlers()const { return columnComparisons; }



    database_query();
    ~database_query();
};

//enum class normalized_column_operator
//{
//    EQUALS,
//    DOES_NOT_EQUAL,
//    LESS_THAN
//};
//
//class normalized_column_comparison
//{
//    std::string column;
//    normalized_column_operator theOperator;
//    database_value value;
//
//public:
//    bool is_order_reversed()const { return value.is_order_reversed(); }
//
//    normalized_column_comparison() :
//        normalized_column_comparison("_", normalized_column_operator::DOES_NOT_EQUAL, 3)
//    {}
//
//    normalized_column_comparison(
//        std::string column,
//        normalized_column_operator theOperator,
//        database_value value
//    )
//    {
//        this->column = column;
//        this->theOperator = theOperator;
//        this->value = value;
//    }
//
//private:
//
//};
//
//class normalized_database_query
//{
//    std::string table;
//    std::vector<normalized_column_comparison> lessComparisons;
//    std::vector<normalized_column_comparison> equalComparisons;
//    std::vector<normalized_column_comparison> unequalComparisons;
//
//public:
//    normalized_database_query(
//        const std::string &table,
//        const std::vector<normalized_column_comparison> equalityComparisons,
//        const std::vector<normalized_column_comparison> inequalityComparisons,
//        const std::vector<normalized_column_comparison> lessComparisons,
//    )
//
//    static normalized_database_query build(const database_query &query)
//    {
//        using namespace std;
//
//        vector<database_column_comparison> comparisons = query.get_column_comparisons();
//
//        vector<normalized_column_comparison> lessComparisons;
//        vector<normalized_column_comparison> equalComparisons;
//        vector<normalized_column_comparison> unequalComparisons;
//
//        for (const database_column_comparison &comparison : comparisons)
//        {
//            database_column_operator theOperator = comparison.get_operator();
//
//            if (false) {}
//            else if (theOperator == database_column_operator::DOES_NOT_EQUAL)
//            {
//                normalized_column_comparison normalized{
//                    comparison.get_column(),
//                    normalized_column_operator::DOES_NOT_EQUAL,
//                    comparison.get_operand()
//                };
//                unequalComparisons.push_back(normalized);
//            }
//            else if (theOperator == database_column_operator::EQUALS)
//            {
//                normalized_column_comparison normalized{
//                    comparison.get_column(),
//                    normalized_column_operator::EQUALS,
//                    comparison.get_operand()
//                };
//                equalComparisons.push_back(normalized);
//            }
//            else if (theOperator == database_column_operator::LESS_THAN)
//            {
//                normalized_column_comparison normalized{
//                    comparison.get_column(),
//                    normalized_column_operator::LESS_THAN,
//                    comparison.get_operand()
//                };
//                lessComparisons.push_back(normalized);
//            }
//            else if (theOperator == database_column_operator::GREATER_THAN)
//            {
//                normalized_column_comparison normalized{
//                    comparison.get_column(),
//                    normalized_column_operator::LESS_THAN,
//                    comparison.get_operand().reverse_order()
//                };
//                lessComparisons.push_back(normalized);
//            }
//            else
//                throw std::runtime_error("could not normalize: operator not supported.");
//
//            normalized_database_query query;
//
//            return query;
//        }
//
//    }
//
//};
