#pragma once
#include <string>
#include "database_type.h"
class column_schema
{
    std::string name;
    database_type type;

public:
    column_schema(const column_schema &other) :
        column_schema(other.name, other.type) {}

    column_schema(std::string name, database_type type) :
        name(name), type(type) {}

    column_schema() {}



    const std::string& get_name() const { return name; }
    database_type get_type()const { return type; }

    void dump(std::ostream &out)const
    {
        out << name << '[';
        dump_database_type(out, type);
        out << ']';

    }

    ~column_schema() {}
};

