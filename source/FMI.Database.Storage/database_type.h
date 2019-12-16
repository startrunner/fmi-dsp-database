#pragma once
#include <iostream>

enum class database_type
{
    BOOLEAN,
    INT32,
    INT64,
    TEXT,
    DOUBLE,
    UUID,
    TIME,
};

void dump_database_type(std::ostream &out, database_type type);

