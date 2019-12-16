#pragma once
#define NOMINMAX
#include <gsl/uuid.h>

namespace lib
{
    using namespace uuids;

    uuid new_id();
    std::string to_string(uuid id);
}