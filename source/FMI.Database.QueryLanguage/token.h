#pragma once
#include <string>
#include <unordered_map>

enum class token_type
{
    UNKNOWN = 0,
    WHITESPACE,

    COMMA,

    IDENTIFIER,
    ASSIGN,
    OPENING_ROUND,
    CLOSING_ROUND,
    OPENING_SQUARE,
    CLOSING_SQUARE,
    LESS_THAN,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN,
    GREATER_THAN_OR_EQUAL,
    EQUAL,
    NOT_EQUAL,

    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,

    KEYWORD_QUERY,
    KEYWORD_AS,
    KEYWORD_JOIN,
    KEYWORD_LEFT,
    KEYWORD_RIGHT,
    KEYWORD_CROSS,
    KEYWORD_PRINT,
    KEYWORD_CREATETABLE,
    KEYWORD_UPDATE,
    KEYWORD_DROPTABLE,
    KEYWORD_DROPCOLUMNS,
    KEYWORD_ADDCOLUMNS,

    LITERAL_INTEGER,
    LITERAL_DOUBLE,
    LITERAL_TEXT,

    TYPE_BOOLEAN,
    TYPE_INT32,
    TYPE_INT64,
    TYPE_TEXT,
    TYPE_DOUBLE,
    TYPE_UUID,
    TYPE_TIME,

    TIME_NOW,
};

struct text_position
{
    const int row, column, charSpan;

    text_position(int row, int column, int charSpan) :
        row(row), column(column), charSpan(charSpan)
    {}

    text_position(const text_position &other) :
        text_position(other.row, other.column, other.charSpan)
    {}

    text_position& operator = (const text_position &other);
};

class token
{
public:
    const std::string text = "";
    const token_type type = token_type::UNKNOWN;
    const text_position position = { 0, 0, 0 };

    token() {}

    token(token_type type, text_position position, std::string text = "") :
        type(type), position(position), text(text)
    {}

    token(const token &other) :
        token(other.type, other.position, other.text)
    {}

    token& operator = (const token &other);
};