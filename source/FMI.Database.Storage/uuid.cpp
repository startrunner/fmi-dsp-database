#include "stdafx.h"
#include "uuid.h"
#include <sstream>
using namespace std;
//using namespace lib;

lib::uuid lib::new_id()
{
    uuid const id = uuids::uuid_system_generator{}();
    return id;
}

std::string lib::to_string(lib::uuid id)
{
    
    ostringstream stream;
    stream << id;

    string text = stream.str();
    return text;
}