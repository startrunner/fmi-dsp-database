#include "stdafx.h"
#include "boost_uuid.h"
using namespace boost::uuids;

uuid lib::new_id()
{
    static auto generator = boost::uuids::random_generator();

    return generator.operator()();
}