#include "stdafx.h"

#include "dbtime.h"
dbtime dbtime::now()
{
    time_t inner = ::time(nullptr);
    dbtime time{ inner };
    return time;
}

std::string dbtime::to_string() const
{
    //fuck this C++ shit
    using namespace std;
    time_t t = this->time;

    //tm *aTime = localtime(&t);
    tm aTime; localtime_s(&aTime, &t);

    int day = aTime.tm_mday;
    int month = aTime.tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
    int year = aTime.tm_year + 1900;

    ostringstream stream;
    stream
        << day << "."
        << month << "."
        << year << " "
        << aTime.tm_hour << ":"
        << aTime.tm_min << ":"
        << aTime.tm_sec;

    string result = stream.str();
    return result;
}