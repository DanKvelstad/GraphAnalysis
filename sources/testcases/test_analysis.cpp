#include "gtest/gtest.h"
#include "support/support.h"

TEST(test_analysis, hello)
{
	ASSERT_EQ("Hello", parse_file("Hello"));
}