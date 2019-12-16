#pragma once
#pragma once
#include "stdafx.h"
#include "database_value.h"
#include "table_row.h"
#include "query_filter.h"
#include "index_schema.h"
#include "primary_key.h"

#include <set>
#include <map>
#include <tuple>
#include <vector>
#include <functional>

class table_index_level
{
    using columns_t = std::vector<std::string>;
    using keyset_t = std::set<primary_key>;
    using filters_t = std::vector<database_column_filter>;

    std::string column;
    std::map<database_value, table_index_level*> childLevels;
    std::map<database_value, keyset_t*> childItems;

public:
    table_index_level(const std::string &column) :column(column) {}

    bool empty()const { return childItems.empty() && childLevels.empty(); }

    void index(
        columns_t::const_iterator columnsBegin,
        columns_t::const_iterator columnsEnd,
        const table_schema &tableSchema,
        table_row row
    );

    void unindex(
        columns_t::const_iterator columnsBegin,
        columns_t::const_iterator columnsEnd,
        const table_schema &tableSchema,
        table_row row
    );

    void query_primary_keys(
        filters_t::const_iterator filtersBegin,
        filters_t::const_iterator filtersEnd,
        std::vector<primary_key> &result
    ) const;


private:
    static void add_child_item_key_range(
        decltype(childItems)::const_iterator begin,
        decltype(childItems)::const_iterator end,
        std::vector<primary_key> &keys
    );

    static void add_child_level_range(
        decltype(childLevels)::const_iterator begin,
        decltype(childLevels)::const_iterator end,
        std::vector<table_index_level const*> &levels
    );

public:
    void dump(std::ostream &out, int indent = 1) const;



    ~table_index_level()
    {
        using namespace std;

        for (const pair<database_value, table_index_level*> &x : this->childLevels)
        {
            delete x.second;
        }

        for (const pair<database_value, keyset_t*> &x : this->childItems)
        {
            delete x.second;
        }
    }

};