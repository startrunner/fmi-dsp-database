#include "token.h"
#include "../include/util/util.h"

text_position & text_position::operator=(const text_position & other)
{
    if (this == &other)return *this;

    setconst(row, other.row);
    setconst(column, other.column);
    setconst(charSpan, other.charSpan);

    return *this;
}

token & token::operator=(const token & other)
{
    if (this == &other) return *this;

    setconst(text, other.text);
    setconst(type, other.type);
    setconst(position, other.position);

    return *this;
}
