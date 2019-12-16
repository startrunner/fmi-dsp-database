#pragma once
#include "stdafx.h"
#include <vector>
#include <map>
#include <string>
#include <vector>
#include "table_schema.h"

class database_schema
{
    std::map<std::string, table_schema> tables;
public:

    database_schema(const std::vector<table_schema> tableSchemas)
    {
        using namespace std;

        map<string, table_schema> byName;

        for (const table_schema &table : tableSchemas)
        {
            const string &name = table.get_name();
            auto found = byName.find(name);
            if (found != byName.end())
                throw std::runtime_error("repeated table schema: " + name);

            byName[name] = table;
        }

        this->tables = byName;
    }

    std::vector<table_schema> get_tables()
    {
        std::vector<table_schema> tables;

        for (const auto &x : this->tables)
        {
            tables.push_back(x.second);
        }

        return tables;
    }
    database_schema() {}
    ~database_schema() {}
};

