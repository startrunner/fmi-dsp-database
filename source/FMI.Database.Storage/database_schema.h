#pragma once
#include <vector>
#include <map>
#include <string>
#include "table_schema.h"

class database_schema
{
    std::string name;
    std::map<std::string, table_schema> tables;
public:

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

