#pragma once
#include "stdafx.h"
#include <vector>
#include <cinttypes>
#include <cstddef>

class database_value
{
protected:
    database_value() {}
public:

    virtual void encode(std::vector<std::byte>  &byteVector);

    virtual ~database_value() {}
};

enum class database_column_comparison
{
    equals,
    not_equals,
    less_than,
    greater_than,
};

class database_index
{
    int id;
    std::vector<database_column_comparison> comparisons;

public:
    database_index(int id, )
};

class database_index_tuple
{
    std::vector<std::byte> bytes;
};


template <typename T>
class database_scalar
{
    T scalar;

    database_scalar() :database_scalar({}) {}
    database_scalar(T scalar) { this->scalar = scalar; }

    void encode(std::vector < std::byte> &byteVector)
    {
        int size = sizeof(T);
        std::byte *bytes = (std::byte*)&scalar;
        for (int i = 0; i < size; i++)
        {
            byteVector.push_back(bytes[i]);
        }
    }

    database_scalar<T> flip() { return { -scalar }; }

    bool operator < (const database_scalar<T> &other)const { return scalar < other.scalar; }
    bool operator >(const database_scalar<T> &other)const { return scalar > other.scalar; }
    bool operator <= (const database_scalar<T> &other)const { return scalar <= other.scalar; }
    bool operator >=(const database_scalar<T> &other)const { return scalar >= other.scalar; }
    bool operator == (const database_scalar<T> &other) const { return scalar == other.scalar; }
    bool operator != (const database_scalar<T> &other)const { return scalar != other.scalar; }

private:

public:

};