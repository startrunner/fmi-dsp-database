#include "stdafx.h"
#include "database_type.h"

void dump_database_type(std::ostream &out, database_type type)
{
    switch (type)
    {
    case database_type::BOOLEAN:
        out << "BOOLEAN";
        break;
    case database_type::INT32:
        out << "INT32";
        break;
    case database_type::INT64:
        out << "INT64";
        break;
    case database_type::TEXT:
        out << "TEXT";
        break;
    case database_type::DOUBLE:
        out << "DOUBLE";
        break;
    case database_type::UUID:
        out << "UUID";
        break;
    case database_type::TIME:
        out << "TIME";
        break;
    default:
        throw std::runtime_error("attempted to dump unsupported database type");
    }
}