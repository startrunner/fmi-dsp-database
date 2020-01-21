#pragma once
#include <string>

bool is_identifier_first_char(char c)
{
    return
        c == '_' || isalpha(c);
}

bool is_digit(char c)
{
    return isalnum(c) && !isalpha(c);
}

bool is_identifier_char(char c)
{
    return isalnum(c) || c == '_';
}

bool is_identifier(const std::string &s)
{
    if (s.empty())return false;
    if (!is_identifier_first_char(s[0]))return false;
    for (char c : s) if (!is_identifier_char(c))return false;

    return true;
}