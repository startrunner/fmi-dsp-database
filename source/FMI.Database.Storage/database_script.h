#pragma once
#include <vector>
#include <string>
#include <util/util.h>
#include "column_schema.h"
#include "query_filter.h"

struct script_instruction
{
    virtual ~script_instruction() {}
    virtual script_instruction *clone() const = 0;
};

struct expression_tree
{
    virtual ~expression_tree() {}
    virtual expression_tree *clone()const = 0;
};

class expression
{
    expression_tree *_tree = nullptr;

public:
    expression() {}
    expression(const expression_tree &tree) :
        _tree(tree.clone())
    {}
    expression(const expression &other) { *this = other; }


    expression& operator = (const expression &other)
    {
        if (this == &other)return *this;

        delete _tree;
        _tree = other._tree->clone();

        return *this;
    }

    virtual ~expression() { delete _tree; }
};

struct drop_table_instruction : script_instruction
{
    const std::string tableName;

    drop_table_instruction() {}
    drop_table_instruction(std::string tableName) :
        tableName(tableName)
    {}
    drop_table_instruction(const drop_table_instruction &other) { *this = other; }

    drop_table_instruction& operator = (const drop_table_instruction &other)
    {
        if (this == &other)return *this;

        setconst(tableName, other.tableName);

        return *this;
    }

    virtual script_instruction* clone()const override
    {
        return new drop_table_instruction(*this);
    }
};

struct create_table_instruction : script_instruction
{
    const std::string name;
    const std::vector<column_schema> specifiedColumns;

    create_table_instruction() {}

    create_table_instruction(const std::string name, const std::vector<column_schema> specifiedColumns) :
        name(name), specifiedColumns(specifiedColumns)
    {}

    create_table_instruction(const create_table_instruction &other) { *this = other; }

    create_table_instruction& operator = (const create_table_instruction &other)
    {
        if (this == &other)return *this;

        setconst(name, other.name);
        setconst(specifiedColumns, other.specifiedColumns);

        return *this;
    }

    virtual script_instruction* clone()const override
    {
        return new create_table_instruction(*this);
    }

};

struct drop_columns_instruction : script_instruction
{
    const std::string table;
    const std::vector<std::string> specifiedColumns;

    drop_columns_instruction() {}
    drop_columns_instruction(const std::string &table, const std::vector<std::string> &specifiedColumns) :
        table(table), specifiedColumns(specifiedColumns)
    {}

    drop_columns_instruction(const drop_columns_instruction &other) { *this = other; }

    drop_columns_instruction& operator = (const drop_columns_instruction &other)
    {
        if (this == &other)return *this;

        setconst(table, other.table);
        setconst(specifiedColumns, other.specifiedColumns);

        return *this;
    }

    script_instruction* clone()const override { return new drop_columns_instruction(*this); }
};

struct add_columns_instruction : script_instruction
{
    const std::string table;
    const std::vector<column_schema> specifiedColumns;

    add_columns_instruction() {}
    add_columns_instruction(const std::string &table, const std::vector<column_schema> &specifiedColumns) :
        table(table), specifiedColumns(specifiedColumns)
    {}
    add_columns_instruction(const add_columns_instruction &other) { *this = other; }

    script_instruction* clone()const override { return new add_columns_instruction(*this); }

    add_columns_instruction& operator = (const add_columns_instruction &other)
    {
        if (this == &other)return *this;

        setconst(table, other.table);
        setconst(specifiedColumns, other.specifiedColumns);

        return *this;
    }
};

struct query_column_selection
{

    const expression expression;
    const std::string column;
    const std::string alias;

    query_column_selection() {}
    query_column_selection(const std::string column, const struct expression &expression) :
        query_column_selection(column, column, expression)
    {}

    query_column_selection(
        const std::string &column,
        const std::string &alias,
        const struct expression &expression
    ) :
        column(column),
        alias(alias),
        expression(expression)
    {

    }

    query_column_selection(const query_column_selection &other) { *this = other; }

    query_column_selection& operator = (const query_column_selection &other)
    {
        if (this == &other)return *this;

        setconst(column, other.column);
        setconst(alias, other.alias);
        setconst(expression, other.expression);

        return *this;
    }
};

struct query_instruction : script_instruction
{
    const query_filter filter;
    const std::vector<query_column_selection> selections;

    query_instruction() {}
    query_instruction(const query_filter &filter, const  std::vector<query_column_selection > selections) :
        filter(filter), selections(selections)
    {}

    query_instruction(const query_instruction &other) { *this = other; }

    query_instruction& operator = (const query_instruction &other)
    {
        if (this == &other)return *this;

        setconst(filter, other.filter);
        setconst(selections, other.selections);

        return *this;
    }

    script_instruction* clone()const override { return new query_instruction(*this); }
};

enum class join_type { LEFT, RIGHT, CROSS = 0 };

struct join_instruction : script_instruction
{
    const join_type type;
    const std::string leftTable, leftTableAlias;
    const std::string rightTable, rightTableAlias;
    const std::string leftColumn, rightColumn;

    const std::map<std::string, std::string> specifiedAliasedColumns;

    join_instruction(
        join_type type,
        std::string leftTable, std::string leftTableAlias,
        std::string rightTable, std::string rightTableAlias,
        std::string leftColumn, std::string rightColumn,
        std::map<std::string, std::string> specifiedAliasedColumns
    ) :
        type(type),
        leftTable(leftTable), leftTableAlias(leftTableAlias),
        rightTable(rightTable), rightTableAlias(rightTableAlias),
        leftColumn(leftColumn), rightColumn(rightColumn),
        specifiedAliasedColumns(specifiedAliasedColumns)
    {}

    join_instruction() :type(join_type::CROSS) {}

    join_instruction(const join_instruction &other) :join_instruction() { *this = other; }

    script_instruction* clone()const override { return new join_instruction(*this); }

    join_instruction& operator = (const join_instruction &other)
    {
        if (this == &other)return *this;

        setconst(type, other.type);
        setconst(leftTable, other.leftTable);
        setconst(leftTableAlias, other.leftTableAlias);
        setconst(rightTable, other.rightTable);
        setconst(rightTableAlias, other.rightTableAlias);
        setconst(leftColumn, other.leftColumn);
        setconst(rightColumn, other.rightColumn);
        setconst(specifiedAliasedColumns, other.specifiedAliasedColumns);

        return *this;
    }


};

struct print_instruction : script_instruction
{
    const std::string table;
    const std::vector<expression> specifiedColumns;

    print_instruction() {}
    print_instruction(const std::string &table, const std::vector<expression> &specifiedColumns) :
        table(table), specifiedColumns(specifiedColumns)
    {}

    print_instruction(const print_instruction &other) { *this = other; }

    script_instruction* clone()const override { return new print_instruction(*this); }

    print_instruction& operator = (const print_instruction &other)
    {
        if (this == &other)return *this;

        setconst(table, other.table);
        setconst(specifiedColumns, other.specifiedColumns);

        return *this;
    }
};

struct insert_instruction : script_instruction
{
    const std::string target;
    const query_instruction query;

    insert_instruction() {}
    insert_instruction(const std::string &target, const query_instruction &query) :
        target(target), query(query)
    {}
    insert_instruction(const insert_instruction &other) { *this = other; }

    script_instruction* clone()const override { return new insert_instruction(*this); }

    insert_instruction& operator = (const insert_instruction &other)
    {
        if (this == &other)return *this;

        setconst(target, other.target);
        setconst(query, other.query);

        return *this;
    }
};

struct assign_query_instruction : script_instruction
{
    const std::string tempTable;
    const query_instruction query;

    assign_query_instruction() {}
    assign_query_instruction(const std::string tempTable, const query_instruction &query)
        :tempTable(tempTable), query(query)
    {}
    assign_query_instruction(const assign_query_instruction &other) { *this = other; }

    assign_query_instruction& operator = (const assign_query_instruction &other)
    {
        if (this == &other)return *this;

        setconst(tempTable, other.tempTable);
        setconst(query, other.query);

        return *this;
    }

    script_instruction* clone() const override { return new assign_query_instruction(*this); }
};

struct assign_join_instruction : script_instruction
{
    const std::string tempTable;
    const join_instruction join;

    assign_join_instruction() {}
    assign_join_instruction(const std::string tempTable, const join_instruction &join)
        :tempTable(tempTable), join(join)
    {}
    assign_join_instruction(const assign_join_instruction &other) { *this = other; }

    assign_join_instruction& operator = (const assign_join_instruction &other)
    {
        if (this == &other)return *this;

        setconst(tempTable, other.tempTable);
        setconst(join, other.join);

        return *this;
    }

    script_instruction* clone() const override { return new assign_join_instruction(*this); }
};

class database_script
{
    std::vector<script_instruction*> instructions;

public:
    database_script() {}
    database_script(const database_script &other) { *this = other; }

    database_script& operator = (const database_script &other)
    {
        if (this == &other)return *this;

        for (script_instruction *x : this->instructions)
            delete x;

        instructions.clear();

        for (script_instruction *x : other.instructions)
            add(*x);

        return *this;
    }


    int count() { return instructions.size(); }
    void add(const script_instruction &instruction)
    {
        instructions.push_back(instruction.clone());
    }

    script_instruction& operator[] (int index) { return *instructions[index]; }
    const script_instruction& operator[] (int index)const { return *instructions[index]; }

    virtual ~database_script()
    {
        for (script_instruction *x : instructions)
            delete x;
    }
};