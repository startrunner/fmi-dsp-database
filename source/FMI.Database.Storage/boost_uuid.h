#pragma once
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>

namespace lib
{
    using uuid = boost::uuids::uuid;
    using boost::uuids::to_string;

    uuid new_id();

    bool operator < (const uuid &x, const uuid &y);

    bool operator > (const uuid &x, const uuid &y);
}