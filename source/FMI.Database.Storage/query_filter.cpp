#include "stdafx.h"
#include "query_filter.h"
bool query_filter::test(const table_row &row)const
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