#pragma once
#include "stdafx.h"
#include "database_value.h"
#include "table_row.h"
#include "query_filter.h"
#include "index_schema.h"
#include "primary_key.h"
#include "table_index_level.h"

#include <set>
#include <map>
#include <tuple>
#include <vector>
#include <functional>

class table_index
{
    const std::string tableName;
    table_index_level *topLevel = nullptr;


public:
    table_index(const std::string &tableName) : tableName(tableName)
    {

    }
    void index(
        table_row row,
        const table_schema &tableSchema,
        const index_schema &indexSchema
    );

    void unindex(
        table_row row,
        const table_schema &tableSchema,
        const index_schema &indexSchema
    );

    std::vector<primary_key> query_primary_keys(query_filter query);

    ~table_index()
    {
        if (topLevel != nullptr)
            delete topLevel;
    }

    void dump(std::ostream &out)const
    {
        out << "index for table (" << tableName << "){";
        if (topLevel != nullptr)
            topLevel->dump(out);
        out << "}";
    }


};