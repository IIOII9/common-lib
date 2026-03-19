#include "pch.h"
#include "gtest/gtest.h"
#include "simple_vector.h"
#include <vector>

std::vector<int> g_vec(10);

class SimpleVectorTest : public testing::Test
{
protected:
	void SetUp() override
	{
    }

	void TearDown() override
	{
    }
    SimpleVector<int> m_vec;
};

TEST_F(SimpleVectorTest, InitialSize)
{

	EXPECT_EQ(m_vec.size(), 0);
	EXPECT_EQ(m_vec.Capacity(), 0);
}
TEST_F(SimpleVectorTest, PushBack)
{
	for (int i = 0; i < 20; i++) {
		m_vec.PushBack(i);
	}
	EXPECT_EQ(m_vec.size(), 20);
	EXPECT_EQ(m_vec.Capacity(), 32);
}