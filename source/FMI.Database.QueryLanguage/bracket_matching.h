#pragma once

#include <vector>
#include "token.h"

bool match_brackets(
    const std::vector<token> &tokens,
    std::vector<int> &openingIndices,
    std::vector<int> &closingIndices
);

bool match_brackets(
    const std::vector<token_type> &types,
    std::vector<int> &openingIndices,
    std::vector<int> &closingIndices
);