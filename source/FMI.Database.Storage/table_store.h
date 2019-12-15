#pragma once
#include <map>
#include <iostream>
#include "primary_key.h"
#include "table_row.h"
#include "database_value.h"
#include "table_schema.h"
#include "table_schema.h"

class table_store
{
    table_schema schema;
    std::map<primary_key, table_row> byPrimaryKey;

public:
    table_store() {}

    table_store(const table_schema &schema)
    {
        this->schema = schema;
    }

    size_t row_count()const { return byPrimaryKey.size(); }

    bool try_create(const table_row row)
    {
        if (!row.check_schema(schema))
            return false;

        primary_key primaryKey = row.get_primary_key(schema);
        auto found = byPrimaryKey.find(primaryKey);

        if (found != byPrimaryKey.end())
        {
            return false;
        }

        byPrimaryKey[primaryKey] = row;
        return true;
    }

    bool try_delete(primary_key primaryKey)
    {
        auto found = byPrimaryKey.find(primaryKey);
        if (found == byPrimaryKey.end())
            return false;


        table_row row = found->second;

        byPrimaryKey.erase(primaryKey);
        return true;
    }

    bool try_get(primary_key primaryKey, table_row *out_row)
    {
        auto found = byPrimaryKey.find(primaryKey);
        if (found == byPrimaryKey.end())
            return false;

        if (out_row != nullptr)
            *out_row = found->second;
        return true;
    }

    bool try_update(primary_key primaryKey, std::map<std::string, database_value> newValues)
    {
        table_row oldRow;
        if (!try_get(primaryKey, &oldRow))
            return false;


        table_row_builder newRow{ oldRow };

        for (std::pair<std::string, database_value> newValue : newValues)
        {
            if (!schema.has_column(newValue.first))
                return false;

            newRow.set(newValue.first, newValue.second);
        }

        byPrimaryKey[primaryKey] = newRow.build();
    }

    void dump(std::ostream &out)const
    {
        schema.dump(out);
    }


public:
    ~table_store() {}
};

