#include "stdafx.h"
#include "table_index_level.h"

void table_index_level::index(
    columns_t::const_iterator columnsBegin,
    columns_t::const_iterator columnsEnd,
    const table_schema &tableSchema,
    table_row row
)
{
    if (columnsBegin == columnsEnd)
        throw std::runtime_error("invalid use of index. at least one column required!");

    std::string currentColumn = *columnsBegin;

    if (currentColumn != this->column)
        throw std::runtime_error("use of incompatible index!");

    database_value currentValue = row.get(currentColumn);

    if (columnsBegin + 1 == columnsEnd)
    {
        auto existing = childItems.find(currentValue);
        if (existing == childItems.end())childItems[currentValue] = new keyset_t();


        keyset_t *keyset = childItems[currentValue];
        primary_key primaryKey = row.get_primary_key(tableSchema);

        keyset->insert(primaryKey);
    }
    else
    {
        auto existing = childLevels.find(currentValue);
        if (existing == childLevels.end())childLevels[currentValue] = new table_index_level(*(columnsBegin + 1));

        table_index_level *childLevel = childLevels[currentValue];
        childLevel->index(columnsBegin + 1, columnsEnd, tableSchema, row);
    }
}

void table_index_level::unindex(
    columns_t::const_iterator columnsBegin,
    columns_t::const_iterator columnsEnd,
    const table_schema &tableSchema,
    table_row row
)
{
    if (columnsBegin == columnsEnd)
        throw std::runtime_error("invalid use of index. at least one column required!");

    std::string currentColumn = *columnsBegin;

    if (currentColumn != this->column)
        throw std::runtime_error("use of incompatible index!");

    database_value currentValue = row.get(currentColumn);

    if (columnsBegin + 1 == columnsEnd)
    {
        auto existing = childItems.find(currentValue);
        if (existing == childItems.end())
            return;

        keyset_t *keyset = childItems[currentValue];
        primary_key primaryKey = row.get_primary_key(tableSchema);

        existing->second->erase(primaryKey);
        if (existing->second->empty())
        {
            delete existing->second;
            childItems.erase(existing);
        }
    }
    else
    {
        auto existing = childLevels.find(currentValue);
        if (existing == childLevels.end())
            return;

        existing->second->unindex(columnsBegin, columnsEnd, tableSchema, row);
        if (existing->second->empty())
        {
            delete existing->second;
            childLevels.erase(existing);
        }
    }
}

//e te tuka si eba mamata sori
template<typename TKey, typename TValue, typename TResult>
static void find_children(
    const typename std::map<TKey, TValue> &children,
    database_column_operator theOperator,
    database_value operand,
    std::function<void(
        typename std::map<TKey, TValue>::const_iterator,
        typename std::map<TKey, TValue>::const_iterator,
        std::vector<TResult>&
        )
    > handleFound,
    std::vector<TResult> &keys
)
{
    if (false) {}
    else if (theOperator == database_column_operator::COLUMN_EQUALS)
    {
        auto equalRange = children.equal_range(operand);
        handleFound(equalRange.first, equalRange.second, keys);
    }
    else if (theOperator == database_column_operator::COLUMN_IS_LESS_THAN)
    {
        auto begin = children.begin();
        auto end = children.lower_bound(operand);
        handleFound(begin, end, keys);
    }
    else if (theOperator == database_column_operator::COLUMN_IS_GREATER_THAN)
    {
        auto begin = children.upper_bound(operand);
        auto end = children.end();
        handleFound(begin, end, keys);
    }
    else if (theOperator == database_column_operator::COLUMN_DOES_NOT_EQUAL)
    {
        find_children(children, database_column_operator::COLUMN_IS_LESS_THAN, operand, handleFound, keys);
        find_children(children, database_column_operator::COLUMN_IS_GREATER_THAN, operand, handleFound, keys);
    }
    else if (theOperator == database_column_operator::COLUMN_IS_LESS_THAN_OR_EQUAL)
    {
        find_children(children, database_column_operator::COLUMN_IS_LESS_THAN, operand, handleFound, keys);
        find_children(children, database_column_operator::COLUMN_EQUALS, operand, handleFound, keys);
    }
    else if (theOperator == database_column_operator::COLUMN_IS_GREATER_THAN_OR_EQUAL)
    {
        find_children(children, database_column_operator::COLUMN_IS_GREATER_THAN, operand, handleFound, keys);
        find_children(children, database_column_operator::COLUMN_EQUALS, operand, handleFound, keys);
    }
    else
    {
        throw std::runtime_error("attempted use of unsupported database column operator :/");
    }
}

void table_index_level::query_primary_keys(
    filters_t::const_iterator filtersBegin,
    filters_t::const_iterator filtersEnd,
    std::vector<primary_key> &result
) const
{
    using namespace std;

    if (filtersBegin == filtersEnd)
        throw std::runtime_error("incorrect usage! at least one column filter required.");

    const database_column_filter &currentFilter = *filtersBegin;
    const std::string &currentColumn = currentFilter.get_column();
    database_value currentOperand = currentFilter.get_operand();
    database_column_operator currentOperator = currentFilter.get_operator();

    if (filtersBegin + 1 == filtersEnd)
    {
        find_children<database_value, keyset_t*, primary_key>(
            childItems,
            currentOperator,
            currentOperand,
            add_child_item_key_range,
            result
            );
    }
    else
    {
        std::vector<table_index_level const*> filteredChildLevels;

        find_children<database_value, table_index_level*, table_index_level const*>(
            childLevels,
            currentOperator,
            currentOperand,
            add_child_level_range,
            filteredChildLevels
            );

        for (const table_index_level *filteredChildLevel : filteredChildLevels)
        {
            filteredChildLevel->query_primary_keys(filtersBegin + 1, filtersEnd, result);
        }
    }
}


void table_index_level::add_child_item_key_range(
    decltype(childItems)::const_iterator begin,
    decltype(childItems)::const_iterator end,
    std::vector<primary_key> &keys
)
{
    for (auto it = begin; it != end; ++it)
    {
        const keyset_t *keyset = it->second;
        for (const primary_key &key : *keyset)
            keys.push_back(key);
    }
}

void table_index_level::add_child_level_range(
    decltype(childLevels)::const_iterator begin,
    decltype(childLevels)::const_iterator end,
    std::vector<table_index_level const*> &levels
)
{
    for (auto it = begin; it != end; ++it)
    {
        const table_index_level *childLevel = it->second;
        levels.push_back(childLevel);
    }
}

void print_indent(std::ostream &out, int indent)
{
    const std::string indentation = "  ";
    for (int i = 0; i < indent; i++)out << indentation;

}

void table_index_level::dump(std::ostream &out, int indent) const
{
    using namespace std;

    if (!childItems.empty())
    {
        int i = 0;
        for (auto childItemEntry : childItems)
        {
            print_indent(out, indent);

            out << childItemEntry.first << "=>[" << endl;
            indent++;

            int j = 0;
            for (const primary_key &primaryKey : *childItemEntry.second)
            {
                print_indent(out, indent);
                out << primaryKey;
                if (j + 1 < childItemEntry.second->size())out << ",";
                out << endl;
                j++;
            }

            print_indent(out, indent);
            out << ']';
            indent--;


            out << std::endl;
            i++;
        }
    }
    if (!childLevels.empty())
    {
        int i = 0;
        for (auto childLevenEntry : childLevels)
        {
            print_indent(out, indent);
            out << childLevenEntry.first << "=>[" << endl;

            const table_index_level &childLevel = *childLevenEntry.second;
            childLevel.dump(out, indent + 1);

            print_indent(out, indent);
            out << ']';


            out << std::endl;
            i++;
        }
    }
}