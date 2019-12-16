#pragma once
#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>


struct dbtime
{
private:
    time_t time;

public:
    dbtime() {}
    dbtime(const dbtime &other)
    {
        this->time = other.time;
    }
    dbtime(time_t time) :time(time) {}

    dbtime& operator = (const dbtime &other)
    {
        if (this == &other)
            return *this;

        this->time = other.time;
        return *this;
    }

    bool operator < (dbtime other)const { return time < other.time; }
    bool operator > (dbtime other)const { return time > other.time; }
    bool operator <= (dbtime other)const { return time <= other.time; }
    bool operator >= (dbtime other)const { return time >= other.time; }
    bool operator == (dbtime other)const { return time == other.time; }
    bool operator != (dbtime other)const { return time != other.time; }

    operator time_t () const { return time; }

    static dbtime now();

    friend std::ostream& operator << (std::ostream& out, const dbtime &dbTime)
    {
        return out << dbTime.to_string();
    }

    std::string to_string() const;
};