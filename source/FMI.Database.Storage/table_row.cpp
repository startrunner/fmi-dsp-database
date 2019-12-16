#include "stdafx.h"
#include "table_row.h"

primary_key table_row::get_primary_key(const table_schema &table)const
{
    assert_schema(table);

    std::vector<column_schema> keySchema = table.get_primary_key();

    std::vector<database_value> keyValues;
    keyValues.reserve(keySchema.size());
    for (const column_schema &columnSchema : keySchema)
    {
        database_value value = values.at(columnSchema.get_name());
        keyValues.push_back(value);
    }

    primary_key key{ keyValues };
    return key;

}

std::vector<std::string> table_row::get_keys()const
{
    std::vector<std::string> keys;

    for (const auto &entry : values)
    {
        keys.push_back(entry.first);
    }

    return keys;
}

database_value table_row::get(const std::string &key)const
{
    return values.at(key);
}
void table_row::assert_schema(const table_schema &table) const
{
    if (!check_schema(table))
        throw std::runtime_error("row does not comply with schema");
}

bool table_row::check_schema(const table_schema &tableSchema)const
{
    if (values.size() > tableSchema.column_count())
        return false;

    for (const auto &entry : values)
    {
        if (!tableSchema.has_column(entry.first))
            return false;

        column_schema columnSchema = tableSchema.get_column(entry.first);

        if (columnSchema.get_type() != entry.second.get_type())
            return false;

    }

    return true;
}

void table_row::dump(std::ostream &out)const
{
    int i = 0;
    for (auto &entry : this->values)
    {
        out << entry.first << ": " << entry.second;
        if (i + 1 < values.size())out << ", ";

        i++;
    }
}