#include "stdafx.h"
#include "table_index.h"

void table_index::index(
    table_row row,
    const table_schema &tableSchema,
    const index_schema &indexSchema
)
{
    using namespace std;

    const vector<string> &specifiedColumns = indexSchema.get_columns();

    if (topLevel == nullptr)
    {
        string firstColumn = specifiedColumns[0];
        topLevel = new table_index_level{ firstColumn };
    }

    topLevel->index(
        specifiedColumns.begin(),
        specifiedColumns.end(),
        tableSchema,
        row
    );
}

void table_index::unindex(
    table_row row,
    const table_schema &tableSchema,
    const index_schema &indexSchema
)
{
    using namespace std;

    if (topLevel == nullptr)return;

    const vector<string> &specifiedColumns = indexSchema.get_columns();

    topLevel->unindex(
        specifiedColumns.begin(),
        specifiedColumns.end(),
        tableSchema,
        row
    );
}

std::vector<primary_key> table_index::query_primary_keys(query_filter query)
{
    if (topLevel == nullptr)
        return {};

    if (query.get_table_name() != tableName)
        throw std::runtime_error("attempt to use incompatible index: different table name!");

    const std::vector<database_column_filter> &filters = query.get_column_fitlers();

    std::vector<primary_key> result;

    topLevel->query_primary_keys(
        filters.begin(),
        filters.end(),
        result
    );

    return result;

}