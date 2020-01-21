#include "token_stream.h"
#include <functional>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <util/charutil.h>
#include <util/collectionutil.h>

using namespace std;


const  std::unordered_map<string, token_type> TERMINAL_TOKEN_TYPES = {
    {",", token_type::COMMA},
    {"=", token_type::EQUAL},
    {"<", token_type::LESS_THAN},
    {">", token_type::GREATER_THAN},
    {"<=", token_type::LESS_THAN_OR_EQUAL},
    {">=", token_type::GREATER_THAN_OR_EQUAL},
    {"+", token_type::ADD},
    {"-", token_type::SUBTRACT},
    {"*", token_type::MULTIPLY},
    {"/", token_type::DIVIDE},
    {"<-", token_type::ASSIGN},
    {"[", token_type::OPENING_SQUARE},
    {"]", token_type::CLOSING_SQUARE},
    {"(", token_type::OPENING_ROUND},
    {")", token_type::CLOSING_ROUND},
    {"AS", token_type::KEYWORD_AS},
    {"QUERY", token_type::KEYWORD_QUERY},
    {"JOIN", token_type::KEYWORD_JOIN},
    {"LEFT", token_type::KEYWORD_LEFT},
    {"RIGHT", token_type::KEYWORD_RIGHT},
    {"CROSS", token_type::KEYWORD_CROSS},
    {"PRINT", token_type::KEYWORD_PRINT},
    {"CREATETABLE", token_type::KEYWORD_CREATETABLE},
    {"DROPTABLE", token_type::KEYWORD_DROPTABLE},
    {"ADDCOLUMNS", token_type::KEYWORD_ADDCOLUMNS},
    {"DROPCOLUMNS", token_type::KEYWORD_DROPCOLUMNS},
    {"BOOLEAN", token_type::TYPE_BOOLEAN},
    {"INT32", token_type::TYPE_INT32},
    {"INT64", token_type::TYPE_INT64},
    {"TEXT", token_type::TYPE_TEXT},
    {"DOUBLE", token_type::TYPE_DOUBLE},
    {"UUID", token_type::TYPE_UUID},
    {"TIME", token_type::TYPE_TIME}
};

vector<string> make_non_identifier_terminals()
{
    vector<string> result;

    for (const auto &x : TERMINAL_TOKEN_TYPES)
    {
        if (is_identifier)
            result.push_back(x.first);
    }

    sort(
        result.begin(),
        result.end(),
        [](const string &x, const string &y) { return x.size() > y.size(); }
    );


    return result;
}

const vector<string> NON_WORD_TERMINALS = make_non_identifier_terminals();

const int MAX_NON_WORD_TERMINAL_LENGTH = NON_WORD_TERMINALS[0].size();

bool token_stream::eof()
{
    try_cache(1);
    return text->eof() && cache.empty();
}

token token_stream::next_token()
{
    if (eof())
        throw runtime_error("EOF reached; Cannot parse any more tokens.");

    token token{};

    if (try_get_word(token))return token;
    if (try_get_whitespace(token))return token;
    if (try_get_non_word_terminal(token))return token;

    if (try_get_numeric_literal(token))return token;
    if (try_get_text_literal(token))return token;

    if (try_get_unknown_character(token))return token;

    throw std::runtime_error("this thing should always return...");
}

bool token_stream::try_get_non_word_terminal(token &token)
{
    try_cache(MAX_NON_WORD_TERMINAL_LENGTH);
    for (const std::string nonIdentifierTerminal : NON_WORD_TERMINALS)
    {
        if (is_prefix_of(nonIdentifierTerminal, cache))
        {
            token_type type = TERMINAL_TOKEN_TYPES.at(nonIdentifierTerminal);
            text_position position{ row, column, (int)nonIdentifierTerminal.size() };
            token = { type, position, nonIdentifierTerminal };
            pop_chars(nonIdentifierTerminal.size());

            return true;
        }
    }

    return false;
}

bool token_stream::try_get_whitespace(token & token)
{
    if (try_cache(1) && isspace(cache.front()))
    {
        int whitespaceLength = 1;
        while (try_cache(whitespaceLength + 1) && isspace(cache[whitespaceLength]))
            whitespaceLength++;

        text_position position{ row, column, whitespaceLength };
        std::string text{ cache.begin(), cache.begin() + whitespaceLength };
        token = { token_type::WHITESPACE, position, text };
        pop_chars(whitespaceLength);

        return true;
    }

    return false;
}

bool token_stream::try_get_word(token & token)
{
    if (try_cache(1) && is_identifier_first_char(cache.front()))
    {
        int wordLength = 1;
        while (try_cache(wordLength + 1) && is_identifier_char(cache[wordLength]))
            wordLength++;

        text_position position{ row, column, wordLength };
        std::string text{ cache.begin(), cache.begin() + wordLength };

        token_type type;
        auto foundTerminal = TERMINAL_TOKEN_TYPES.find(text);
        if (foundTerminal != TERMINAL_TOKEN_TYPES.end())
            type = foundTerminal->second;
        else
            type = token_type::IDENTIFIER;

        token = { type, position, text };
        pop_chars(wordLength);

        return true;
    }

    return false;
}

bool token_stream::try_get_numeric_literal(token & token)
{
    if (!try_cache(1))return false;
    if (!is_digit(cache.front()))return false;

    int length = 1;
    bool hadDot = false;

    while (true)
    {
        bool condition =
            try_cache(length + 1) &&
            (is_digit(cache[length]) || (!hadDot && cache[length] == '.'));
        if (!condition)break;

        if (cache[length] == '.') hadDot = true;

        length++;
    }

    if (cache[length - 1] == '.')return false;

    std::string text{ cache.begin(), cache.begin() + length };
    text_position position{ row, column, length };

    token_type type;
    if (hadDot)type = token_type::LITERAL_DOUBLE;
    else type = token_type::LITERAL_INTEGER;

    pop_chars(length);

    token = { type, position, text };
    return true;
}

bool token_stream::try_get_text_literal(token & token)
{
    if (!try_cache(2))return false;
    if (cache.front() != '"')return false;

    int length = 2;
    while (try_cache(length + 1) && cache[length - 1] != '"')
        length++;

    if (cache[length - 1] != '"')return false;

    string text{ cache.begin(), cache.begin() + length };
    text_position position{ row, column, length };

    pop_chars(length);

    token = { token_type::LITERAL_TEXT, position, text };
    return true;
}

bool token_stream::try_get_unknown_character(token &token)
{
    if (!try_cache(1))return false;

    string text{ cache.begin(), cache.begin() + 1 };
    text_position position{ row, column, 1 };
    token = { token_type::UNKNOWN, position, text };

    pop_chars(1);

    return true;
}

bool token_stream::try_cache(int count)
{
    int character = -1;
    while (cache.size() < count && (character = text->get()) > 0)
        cache.push_back((char)character);
    return cache.size() >= count;
}

void token_stream::pop_chars(int count)
{
    if (!try_cache(count))
        throw std::runtime_error("Can't pop [" + to_string(count) + "] chars!");

    for (int i = 0; i < count; i++)
    {
        char c = cache.front();
        cache.pop_front();

        if (c == '\n') { row++; column = 1; }
        else column++;
    }
}

std::vector<token> skip_whitespace(const std::vector<token> tokens)
{
    std::vector<token> meaningful;

    for (const token &x : tokens)
    {
        if (x.type == token_type::WHITESPACE)continue;

        meaningful.push_back(x);
    }

    return meaningful;
}

std::vector<token> tokenize_text(const std::string & text)
{
    istringstream stream{ text };
    return tokenize_stream(stream);
}

std::vector<token> tokenize_file(const std::string & path)
{
    ifstream stream{ path };
    return tokenize_stream(stream);
}

std::vector<token> tokenize_stream(istream &stream)
{
    token_stream tokenizer{ stream };

    vector<token> tokens;

    while (!tokenizer.eof())
    {
        struct token token = tokenizer.next_token();
        tokens.push_back(token);
    }

    return tokens;

}