#include "gtest/gtest.h"

// all tests are named "ExampleTestOne" or "ExampleTestTwo"
// but they are in different test suites

// these tests are in test suite one
TEST(ExampleSuiteOne, ExampleTestOne)
{
    EXPECT_STREQ("hello world!", "hello world!");
}

TEST(ExampleSuiteOne, ExampleTestTwo)
{
    EXPECT_EQ(3 * 3, 9);
}

// these tests are in test suite two
TEST(ExampleSuiteTwo, ExampleTestOne)
{
    EXPECT_STRNE("foo", "bar");
}

TEST(ExampleSuiteTwo, ExampleTestTwo)
{
    EXPECT_NE(4 * 4, 15);
}