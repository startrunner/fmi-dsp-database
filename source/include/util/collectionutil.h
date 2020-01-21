#pragma once
#include <vector>
#include <string>

template<typename T1, typename T2>
bool is_prefix_of(const T1 &x, const T2 &y)
{
    if (x.size() > y.size())return false;

    auto itX = x.begin();
    auto itY = y.begin();

    while (itX != x.end())
    {
        if ((*itX) != (*itY))
            return false;

        itX++;
        itY++;
    }

    return true;
}