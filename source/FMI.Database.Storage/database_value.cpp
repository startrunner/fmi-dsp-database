#include "stdafx.h"
#include "database_value.h"
#include "database_type.h"
std::ostream& operator << (std::ostream& out, const database_value &toPrint)
{
    using namespace std;

    database_value::value_t value = toPrint.value;
    switch (toPrint.type)
    {
    case database_type::BOOLEAN:
        out << get<bool>(value) ? "true" : "false";
        break;
    case database_type::DOUBLE:
        out << get<double>(value);
        break;
    case database_type::INT32:
        out << get<int32_t>(value);
        break;
    case database_type::INT64:
        out << get<int64_t>(value);
        break;
    case database_type::TEXT:
        out << get<string>(value);
        break;
    case database_type::TIME:
        out << get<dbtime>(value);
        break;
    case database_type::UUID:
        out << get<boost::uuids::uuid>(value);
        break;

    default:
        break;
    }
    return out;
}