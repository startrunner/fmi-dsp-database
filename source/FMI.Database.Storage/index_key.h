#pragma once
#include <vector>
#include <stdexcept>
#include "database_value.h"
class index_key
{
    std::vector<database_value> values;
public:

    index_key() {}
    index_key(const index_key &other) :values(other.values) {}
    index_key(const std::vector<database_value> &values) :values(values) {}

    bool operator < (const index_key &other)const
    {
        assert_comparable(other);
        return values < other.values;
    }

    bool operator > (const index_key &other)const
    {
        assert_comparable(other);
        return values > other.values;
    }

private:

    void assert_comparable(const index_key &other)const
    {
        if (values.size() != other.values.size())
            throw std::runtime_error("comparison of incompatible index keys.");
    }

public:
    ~index_key() {}
};

