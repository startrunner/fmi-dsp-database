#pragma once
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>

namespace lib
{
    using  boost::uuids::uuid;
    using boost::uuids::to_string;
    using namespace boost::uuids;

    uuid new_id();
}