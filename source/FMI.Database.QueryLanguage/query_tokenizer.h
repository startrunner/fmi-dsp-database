#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include "token.h"

struct text_position
{
    const int row, column, index;

    text_position(int row = 0, int column = 0, int index = 0) :
        row(row), column(column), index(index)
    {}

    text_position(const text_position &other) :
        text_position(other.row, other.column, other.index)
    {}
};

class query_token
{
    std::string content;
    text_position position;
    token_type type = token_type::UNKNOWN;


public:

    const text_position& get_position()const { return position; }
    const std::string& get_content()const { return content; }


    query_token() {}
    query_token(const text_position &position, const std::string &content) :
        position(position), content(content)
    {}

private:
    static token_type determine_type(const std::string &content)
    {


    }
};

class query_tokenizer
{

};