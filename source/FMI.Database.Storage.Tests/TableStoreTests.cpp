#include "pch.h"
#include "../FMI.Database.Storage/database_storage_all.h"
using namespace std;
using namespace lib;


TEST(Twat, TableStore)
{
    const string TableName = "Tbl";

    vector<column_schema> columnSchemas{
        {"ColUuid", database_type::UUID},
        {"Col32", database_type::INT32 },
        {"Col64", database_type::INT64 },
        {"ColText", database_type::TEXT},
        {"ColTime", database_type::TIME},
    };

    vector<string> primaryKey{ "ColUuid" };

    table_schema tableSchema{ TableName, columnSchemas, primaryKey };

    table_store tableStore{ tableSchema };


    table_row row1 =
        table_row_builder{}
        .set("ColUuid", new_id())
        .set("Col32", 16)
        .set("ColText", "Gosho"s)
        .set("ColTime", dbtime::now())
        .build();

    EXPECT_TRUE(tableStore.try_create(row1));
    EXPECT_EQ(tableStore.row_count(), 1);

    EXPECT_EQ(1, 1);
    EXPECT_TRUE(true);
}