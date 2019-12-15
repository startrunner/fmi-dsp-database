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

    bool operator <= (const primary_key &other)const
    {
        for (int i = 0; i < std::min(values.size(), other.values.size()); i++)
        {
            if (values[i] < other.values[i])return true;
            if (values[i] > other.values[i])return false;
        }
        return values.size() <= other.values.size();
    }

    bool operator >= (const primary_key &other)const
    {
        for (int i = 0; i < std::min(values.size(), other.values.size()); i++)
        {
            if (values[i] < other.values[i])return false;
            if (values[i] > other.values[i])return true;
        }
        return values.size() >= other.values.size();
    }

    bool operator == (const primary_key &other)const
    {
        return values == other.values;
    }

    bool operator != (const primary_key &other)const
    {
        return values != other.values;
    }

    bool operator < (const primary_key &other)const
    {
        for (int i = 0; i < std::min(values.size(), other.values.size()); i++)
        {
            if (values[i] < other.values[i])return true;
            if (values[i] > other.values[i])return false;
        }
        return values.size() < other.values.size();
    }

    bool operator > (const primary_key &other)const
    {
        for (int i = 0; i < std::min(values.size(), other.values.size()); i++)
        {
            if (values[i] < other.values[i])return false;
            if (values[i] > other.values[i])return true;
        }
        return values.size() > other.values.size();
    }

private:

    template<typename CompareItems, typename CompareLengths> bool compare(const primary_key &other)const
    {
        static const CompareItems compareItems{};
        static const CompareLengths compareLengths{};

        for (int i = 0; i < std::min(values.size(), other.values.size()); i++)
        {
            if (!compareItems(values[i], other.values[i])) return false;
        }

        return compareLengths(values.size(), other.values.size());
    }

    void assert_comparable(const primary_key &other)const
    {
        if (values.size() != other.values.size())
            throw std::runtime_error("comparison of incompatible index keys.");
    }

public:
    ~primary_key() {}
};

