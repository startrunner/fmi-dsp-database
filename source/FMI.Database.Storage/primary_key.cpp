#include "stdafx.h"
#include "primary_key.h"

bool primary_key::operator <= (const primary_key &other)const
{
    for (int i = 0; i < std::min(values.size(), other.values.size()); i++)
    {
        if (values[i] < other.values[i])return true;
        if (values[i] > other.values[i])return false;
    }
    return values.size() <= other.values.size();
}

bool primary_key::operator >= (const primary_key &other)const
{
    for (int i = 0; i < std::min(values.size(), other.values.size()); i++)
    {
        if (values[i] < other.values[i])return false;
        if (values[i] > other.values[i])return true;
    }
    return values.size() >= other.values.size();
}

bool primary_key::operator < (const primary_key &other)const
{
    for (int i = 0; i < std::min(values.size(), other.values.size()); i++)
    {
        if (values[i] < other.values[i])return true;
        if (values[i] > other.values[i])return false;
    }
    return values.size() < other.values.size();
}

bool primary_key:: operator > (const primary_key &other)const
{
    for (int i = 0; i < std::min(values.size(), other.values.size()); i++)
    {
        if (values[i] < other.values[i])return false;
        if (values[i] > other.values[i])return true;
    }
    return values.size() > other.values.size();
}

std::ostream& operator << (std::ostream &out, const primary_key &key)
{
    const auto &values = key.values;

    out << "PK[";
    for (int i = 0; i < values.size(); i++)
    {
        out << values[i];
        if (i + 1 < values.size())out << ", ";
    }
    out << "]";

    return out;
}