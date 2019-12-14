#include "stdafx.h"

#include "database_value.h"
dbtime dbtime::now()
{
    time_t inner = ::time(nullptr);
    dbtime time{ inner };
    return time;
}