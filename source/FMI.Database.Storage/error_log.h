#pragma once
#include <string>

struct error_log
{
    void log(const std::string &error) {}

    virtual ~error_log() {}
};