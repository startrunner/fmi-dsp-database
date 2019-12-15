#pragma once
#include <map>
#include <iostream>
#include "primary_key.h"
#include "table_row.h"
#include "database_value.h"
#include "table_schema.h"
#include "table_index.h"
#include "table_schema.h"

class table_store
{
    table_schema schema;
    std::map<primary_key, table_row> byPrimaryKey;
    std::map<index_schema, table_index*>  builtIndices;

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

        index(row);

        return true;
    }

    bool try_delete(primary_key primaryKey)
    {
        auto found = byPrimaryKey.find(primaryKey);
        if (found == byPrimaryKey.end())
            return false;


        table_row row = found->second;

        byPrimaryKey.erase(primaryKey);

        unindex(row);

        return true;
    }

    table_row get(const primary_key &primaryKey)const
    {
        table_row row;
        if (!try_get(primaryKey, row))
            throw std::runtime_error("could not fetch row by primary key :/");
        return row;
    }

    bool try_get(const primary_key &primaryKey, table_row &out_row)const
    {
        auto found = byPrimaryKey.find(primaryKey);
        if (found == byPrimaryKey.end())
            return false;

        out_row = found->second;
        return true;
    }

    bool try_update(primary_key primaryKey, std::map<std::string, database_value> newValues)
    {
        table_row oldRow;
        if (!try_get(primaryKey, oldRow))
            return false;


        table_row_builder newRowBuilder{ oldRow };

        for (std::pair<std::string, database_value> newValue : newValues)
        {
            if (!schema.has_column(newValue.first))
                return false;

            newRowBuilder.set(newValue.first, newValue.second);
        }

        table_row newRow = newRowBuilder.build();

        unindex(oldRow);
        index(newRow);

        byPrimaryKey[primaryKey] = newRow;
        return true;
    }

    void dump(std::ostream &out)const
    {
        schema.dump(out);

        int indexIndex = 0;
        for (const std::pair <index_schema, table_index*> &indexEntry : builtIndices)
        {
            const index_schema &indexSchema = indexEntry.first;
            const table_index *index = indexEntry.second;

            indexSchema.dump(out);
            out << std::endl;
            index->dump(out);
        }

        for (const auto &entry : byPrimaryKey)
        {
            out << entry.first << "=>";
            entry.second.dump(out);
            out << std::endl;
        }
    }

    std::vector<table_row> get_rows(const query_filter &filter)
    {
        std::vector<primary_key> primaryKeys = get_primary_keys(filter);

        std::vector<table_row> rows = get_rows(primaryKeys);

        return rows;
    }

    std::vector<table_row> get_rows(const std::vector<primary_key> &primaryKeys) const
    {
        std::vector<table_row> rows;

        for (const primary_key &key : primaryKeys)
        {
            table_row row = get(key);
            rows.push_back(row);
        }

        return rows;
    }

private:

    std::vector<primary_key> get_primary_keys(const query_filter &filter)
    {
        std::vector<primary_key> primaryKeys;

        if (!filter.uses_index())
        {
            for (const auto &entry : this->byPrimaryKey)
            {
                const table_row &row = entry.second;
                const primary_key &key = entry.first;

                if (filter.test(row))
                    primaryKeys.push_back(key);
            }
            return primaryKeys;
        }

        build_index(filter);

        index_schema indexSchema = build_normalized_index_schema(filter);
        table_index &index = *builtIndices[indexSchema];

        primaryKeys = index.query_primary_keys(filter);

        return primaryKeys;
    }

    void build_index(const query_filter &filter)
    {
        if (!filter.uses_index())
            throw std::runtime_error("attempted to build index for non-indexed query :/");

        index_schema schema =
            build_normalized_index_schema(filter);

        build_index(schema);
    }

    static index_schema build_normalized_index_schema(const query_filter &filter)
    {
        std::vector<std::string> columns;

        const std::vector<database_column_filter> &filters = filter.get_column_fitlers();

        for (const database_column_filter &filter : filters)
        {
            columns.push_back(filter.get_column());
        }

        index_schema unNormalized{ columns };
        index_schema normalized = unNormalized.normalize();

        return normalized;
    }

    void build_index(const index_schema &schema)
    {
        index_schema normal = schema.normalize();

        auto existing = builtIndices.find(normal);
        if (existing != builtIndices.end())
            return;

        table_index &index = *new table_index(this->schema.get_name());
        builtIndices[normal] = &index;

        for (const auto &entry : this->byPrimaryKey)
        {
            const table_row &row = entry.second;
            index.index(row, this->schema, schema);
        }
    }

    void index(const table_row &row)
    {
        for (const std::pair<index_schema, table_index*>indexEntry : builtIndices)
        {
            table_index *index = indexEntry.second;
            const index_schema &indexSchema = indexEntry.first;

            index->index(row, schema, indexSchema);
        }
    }

    void unindex(const table_row &row)
    {
        for (const std::pair<index_schema, table_index*>indexEntry : builtIndices)
        {
            table_index *index = indexEntry.second;
            const index_schema &indexSchema = indexEntry.first;

            index->unindex(row, schema, indexSchema);
        }
    }


public:
    ~table_store()
    {
        for (auto &indexEntry : builtIndices)
        {
            delete indexEntry.second;
        }
    }
};

