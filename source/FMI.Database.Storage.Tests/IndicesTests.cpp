#include "pch.h"
#include "../FMI.Database.Storage/database_storage_all.h"
#include "gtest/gtest.h"
using namespace std;
using namespace lib;

TEST(Indices, ShouldHaveSameResultAsUnindexedQuery)
{
    const int EntryCount = 1000000;
    srand(4554);

    const string TableName = "TheTable";

    vector<column_schema> columnSchemas{
        {"ID", database_type::UUID},
        {"Col1", database_type::INT32 },
        {"Col2", database_type::INT32 },
        {"Col3", database_type::INT32},
        {"Col4", database_type::INT32},
        {"Col5", database_type::INT32}
    };

    vector<string> primaryKey{ "ID" };

    table_schema tableSchema{ TableName, columnSchemas, primaryKey };

    table_store tableStore{ tableSchema };

    for (int i = 0; i < EntryCount; i++)
    {
        table_row row =
            table_row_builder{}
            .set("ID", new_id())
            .set("Col1", rand())
            .set("Col2", rand())
            .set("Col3", rand())
            .set("Col4", rand())
            .set("Col5", rand())
            .build();

        EXPECT_TRUE(tableStore.try_create(row));
    }

    query_filter unindexedFilter{
        TableName,
        std::vector<database_column_filter>{
            database_column_filter{
                "Col1",
                database_column_operator::COLUMN_IS_GREATER_THAN,
                1000
            },
            database_column_filter{
                "Col2",
                database_column_operator::COLUMN_IS_LESS_THAN,
                5000
            },
            database_column_filter{
                "Col3",
                database_column_operator::COLUMN_IS_GREATER_THAN_OR_EQUAL,
                3000
            },
                database_column_filter{
                "Col4",
                database_column_operator::COLUMN_DOES_NOT_EQUAL,
                1000
            },
            database_column_filter{
                "Col5",
                database_column_operator::COLUMN_IS_LESS_THAN,
                7000
            }
        },
        false
    };

    query_filter indexedFilter{
        unindexedFilter.get_table_name(),
        unindexedFilter.get_column_fitlers(),
        true
    };

    vector<table_row> fromUnindexed = tableStore.get_rows(unindexedFilter);
    vector<table_row> fromIndexed = tableStore.get_rows(indexedFilter);

    EXPECT_FALSE(fromIndexed.size() == 0);

    std::sort(fromIndexed.begin(), fromIndexed.end());
    std::sort(fromUnindexed.begin(), fromUnindexed.end());

    bool areEqual = fromIndexed == fromUnindexed;

    EXPECT_TRUE(areEqual);

    std::cout << "ura";
}