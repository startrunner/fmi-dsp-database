
#include "bracket_matching.h"
#include <map>
#include <stack>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct stack_item
{
    token_type type = token_type::UNKNOWN;
    int index = -1;

    stack_item() {}
    stack_item(token_type type, int index) :
        type(type), index(index)
    {}
};

bool match_brackets(
    const vector<token> &tokens,
    vector<int> &openingIndices,
    vector<int> &closingIndices
)
{
    vector <token_type> types;
    for (const token &token : tokens)
        types.push_back(token.type);

    return match_brackets(types, openingIndices, closingIndices);
}

const unordered_map<token_type, token_type> CLOSING_OF{
    {token_type::OPENING_ROUND,  token_type::CLOSING_ROUND },
    {token_type::OPENING_SQUARE, token_type::CLOSING_SQUARE}
};

const unordered_map<token_type, token_type> OPENING_OF{
    {token_type::CLOSING_ROUND, token_type::OPENING_ROUND},
    {token_type::CLOSING_SQUARE, token_type::OPENING_SQUARE}
};



bool match_brackets(
    const vector<token_type> &types,
    vector<int> &openingIndices,
    vector<int> &closingIndices
)
{
    openingIndices.resize(types.size());
    closingIndices.resize(types.size());
    fill(openingIndices.begin(), openingIndices.end(), -1);
    fill(closingIndices.begin(), closingIndices.end(), -1);


    stack<stack_item> matchStack;

    for (int i = 0; i < types.size(); i++)
    {
        token_type current = types[i];

        bool isOpening =
            CLOSING_OF.find(current) != CLOSING_OF.end();

        if (isOpening)
        {
            matchStack.emplace(current, i);
            continue;
        }

        bool isClosing =
            OPENING_OF.find(current) != OPENING_OF.end();

        if (isClosing)
        {
            decltype(CLOSING_OF.cbegin()) matchingPair;

            bool matched =
                !matchStack.empty() &&
                (matchingPair = CLOSING_OF.find(matchStack.top().type))->second == current;

            if (matched)
            {
                int openingIndex = matchStack.top().index;
                int closingIndex = i;

                closingIndices[openingIndex] = closingIndex;
                openingIndices[closingIndex] = openingIndex;

                matchStack.pop();
            }
            else
                matchStack.emplace(current, i);
        }

    }

    return matchStack.empty();
}