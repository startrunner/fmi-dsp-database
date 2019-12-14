#include "pch.h"
#include "gtest/gtest.h"

using namespace std;

template<typename T> void less_than_test(T lesser, T greater);

TEST(DatabaseValue, Compare_Less_INT32)
{
    less_than_test(3, 4);
}

TEST(DatabaseValue, Compare_Less_TIME)
{
    less_than_test(3LL, 4LL);
}

TEST(DatabaseValue, Compare_Less_TEXT)
{
    less_than_test("a", "b");
}

TEST(DatabaseValue, StringifyTime)
{
    time_t _t;
    _t = time(nullptr);
    dbtime t{ _t };
    std::string str = t.to_string();

    ;
}

template<typename T> void less_than_test(T lesser, T greater)
{
    database_value x = lesser;
    database_value y = greater;

    EXPECT_TRUE(x < y);
    EXPECT_FALSE(y < x);

    x = x.reverse_order();
    y = y.reverse_order();

    EXPECT_FALSE(x < y);
    EXPECT_TRUE(y < x);
}