#pragma once
#include <deque>
#include <vector>
#include "token.h"
#include "bracket_matching.h"
#include "../FMI.Database.Storage/database_script.h"

class script_parser
{
    class data_t
    {
        int offset = 0;
        std::deque<token> tokens;
        std::deque<int> openingIndices;
        std::deque<int> closingIndices;

    public:

        const token& operator[](int index)const { return tokens[index]; }

        int opening(int index)const { return openingIndices[index - offset] - offset; }

        int closing(int index)const { return closingIndices[index - offset] - offset; }

        int count()const { return tokens.size(); }

        void pop(int count)
        {
            if (count > tokens.size)
                throw std::runtime_error("Can't pop " + std::to_string(count) + " tokens!");

            offset += count;
            for (int i = 0; i < tokens.size(); i++)
            {
                tokens.pop_front();
                openingIndices.pop_front();
                closingIndices.pop_front();
            }
        }

        data_t() {}

        data_t(const std::vector<token> tokens)
        {
            std::vector<token> meaningful;
            for (const token &x : tokens)
            {
                if (x.type != token_type::WHITESPACE)
                    meaningful.push_back(x);
            }

            std::vector<int> opening, closing;
            match_brackets(meaningful, opening, closing);

            this->tokens = { meaningful.begin(), meaningful.end() };
            this->openingIndices = { opening.begin(), opening.end() };
            this->closingIndices = { closing.begin(), closing.end() };
        }
    }data;

    database_script parse()
    {
        database_script result;

        script_instruction *instruction = nullptr;

        while (try_parse(0, instruction))
        {
            result.add(*instruction);
            delete instruction;
        }

        return result;
    }

private:
    bool try_parse(int at, script_instruction *& instruction)
    {
        bool isAssignmentToTemporaryTable =
            data.count() > 2 && data[at + 0].type == token_type::IDENTIFIER && data[at + 1].type == token_type::ASSIGN;

        if (isAssignmentToTemporaryTable)
        {
            script_instruction *inner = nullptr;
            if (try_parse(2, inner))
            {
                if(query_instruction )

                delete inner;
            }
        }

        instruction = nullptr;
    }
};