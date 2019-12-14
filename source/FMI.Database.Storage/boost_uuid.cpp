#include "stdafx.h"
#include "boost_uuid.h"
using namespace boost::uuids;

uuid lib::new_id()
{
    auto generator = boost::uuids::random_generator();

    return generator();
}

bool lib::operator<(const uuid & x, const uuid & y)

{
    return to_string(x) < to_string(y);
}

bool lib::operator>(const uuid & x, const uuid & y)

{
    return to_string(x) < to_string(y);
}