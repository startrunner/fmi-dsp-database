#include "pch.h"
#include <fstream>
#include <iostream>
#include <string>
#include <util/collectionutil.h>
#include "../FMI.Database.QueryLanguage/token.h"
#include "../FMI.Database.QueryLanguage/token_stream.h"
#include "../FMI.Database.QueryLanguage/bracket_matching.h"

using namespace std;

TEST(Tokenization, CorrectTokenTypes)
{
    using tt = token_type;
    vector<token_type> expectedTypes{
        tt::IDENTIFIER,
        tt::ASSIGN,
        tt::KEYWORD_QUERY,
        tt::IDENTIFIER,
        tt::OPENING_ROUND,

        tt::IDENTIFIER,
        tt::EQUAL,
        tt::LITERAL_TEXT,
        tt::COMMA,

        tt::IDENTIFIER,
        tt::EQUAL,
        tt::LITERAL_TEXT,
        tt::COMMA,

        tt::IDENTIFIER,
        tt::LESS_THAN,
        tt::LITERAL_INTEGER,
        tt::COMMA,

        tt::IDENTIFIER,
        tt::GREATER_THAN,
        tt::LITERAL_DOUBLE,
        tt::COMMA,

        tt::IDENTIFIER,
        tt::LESS_THAN,
        tt::IDENTIFIER,
        tt::COMMA,

        tt::UNKNOWN, tt::UNKNOWN, tt::UNKNOWN,
        tt::CLOSING_ROUND,

        tt::OPENING_SQUARE,
        tt::IDENTIFIER,
        tt::KEYWORD_AS,
        tt::IDENTIFIER,
        tt::COMMA,
        tt::IDENTIFIER,
        tt::ADD,
        tt::IDENTIFIER,
        tt::KEYWORD_AS,
        tt::IDENTIFIER,
        tt::CLOSING_SQUARE,


        tt::IDENTIFIER,
        tt::ASSIGN,
        tt::KEYWORD_JOIN,
        tt::OPENING_ROUND,
        tt::KEYWORD_LEFT,
        tt::COMMA,
        tt::IDENTIFIER
    };

    std::vector<token> tokens = skip_whitespace(tokenize_file("query.txt"));

    std::vector<token_type> types;
    for (const token &x : tokens)types.push_back(x.type);

    for (int i = 0; i < min(types.size(), expectedTypes.size()); i++)
    {
        EXPECT_EQ(types[i], expectedTypes[i]);
    }
}

void assert_matched_brackets(string text, vector<pair<int, int>> pairs);

TEST(BracketMatching, CorrentBracketMatching_1)
{
    assert_matched_brackets("()", { {0, 1} });
}

TEST(BracketMatching, CorrentBracketMatching_2)
{
    assert_matched_brackets("([]())", { {0, 5}, {1, 2}, {3, 4} });
}

TEST(BracketMatching, CorrentBracketMatching_None)
{
    assert_matched_brackets("pesho", {});
}

void assert_matched_brackets(string text, vector<pair<int, int>> pairs)
{
    std::vector<token> tokens = tokenize_text(text);

    vector<int> openingIndices;
    vector<int> closingIndices;

    EXPECT_TRUE(match_brackets(tokens, openingIndices, closingIndices));

    std::set<int> matchedIndices;
    for (auto pair : pairs)
    {
        matchedIndices.insert(pair.first);
        matchedIndices.insert(pair.second);
    }

    for (auto pair : pairs)
    {
        EXPECT_EQ(closingIndices[pair.first], pair.second);
        EXPECT_EQ(openingIndices[pair.second], pair.first);
    }

    for (int i = 0; i < openingIndices.size(); i++)
    {
        bool currentMatched = matchedIndices.find(i) != matchedIndices.end();

        if (!currentMatched)
        {
            EXPECT_EQ(openingIndices[i], -1);
            EXPECT_EQ(closingIndices[i], -1);
        }
        else
        {
            bool openingFound =
                matchedIndices.find(openingIndices[i]) != matchedIndices.end();

            bool closingFound =
                matchedIndices.find(closingIndices[i]) != matchedIndices.end();

            EXPECT_TRUE(openingFound != closingFound);

        }
    }
}