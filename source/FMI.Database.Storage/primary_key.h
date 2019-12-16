#pragma once
#include <vector>
#include <stdexcept>
#include "database_value.h"
class primary_key
{
    std::vector<database_value> values;
public:

    primary_key() {}
    primary_key(const primary_key &other) :values(other.values) {}
    primary_key(const std::vector<database_value> &values) :values(values) {}

    bool operator <= (const primary_key &other)const;
    bool operator >= (const primary_key &other)const;
    bool operator < (const primary_key &other)const;
    bool operator > (const primary_key &other)const;
    bool operator == (const primary_key &other)const { return values == other.values; }
    bool operator != (const primary_key &other)const { return values != other.values; }

    friend std::ostream& operator << (std::ostream &out, const primary_key &key);

public:
    ~primary_key() {}
};

