#pragma once
#include "token.h"
#include <deque>
#include <vector>

class token_stream
{
    int row = 1, column = 1;
    std::deque<char> cache;
    std::istream *text;

public:
    bool eof();
    token next_token();

    token_stream(std::istream &text) :
        text(&text)
    {}

private:
    bool try_get_non_word_terminal(token &token);
    bool try_get_whitespace(token &token);
    bool try_get_word(token &token);
    bool try_get_numeric_literal(token &token);
    bool try_get_text_literal(token &token);
    bool try_get_unknown_character(token &token);
    bool try_cache(int count);
    void pop_chars(int count);
};

std::vector<token> skip_whitespace(const std::vector<token> tokens);
std::vector<token> tokenize_stream(std::istream &stream);
std::vector<token> tokenize_text(const std::string &text);
std::vector<token> tokenize_file(const std::string &path);