#pragma once

template<typename T> void setconst(const T &left, const T &right)
{
    T *ptr = (T*)(&left);
    (*ptr) = right;
}