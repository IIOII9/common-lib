#include "pch.h"
#include "gtest/gtest.h"
#include "SimpleList.h"
#include <vector>

class SimpleListTest : public testing::Test
{
protected:
    void SetUp() override
    {
        // 按需初始化
    }

    void TearDown() override
    {
    }

    std::vector<int> ToVector(const SimpleList<int>& list)
    {
        std::vector<int> values;
        for (auto it = list.Begin(); it != list.End(); ++it) {
            values.push_back(*it);
        }
        return values;
    }

protected:
    SimpleList<int> m_list;
};

TEST_F(SimpleListTest, DefaultConstructedListIsEmpty)
{
    EXPECT_EQ(m_list.Size(), 0u);
    EXPECT_EQ(m_list.Begin(), m_list.End());
}

TEST_F(SimpleListTest, PushBackWorks)
{
    m_list.PushBack(1);
    m_list.PushBack(2);
    m_list.PushBack(3);

    EXPECT_EQ(m_list.Size(), 3u);

    std::vector<int> expected{1, 2, 3};
    EXPECT_EQ(ToVector(m_list), expected);
}

TEST_F(SimpleListTest, PushFrontWorks)
{
    m_list.PushFront(3);
    m_list.PushFront(2);
    m_list.PushFront(1);

    EXPECT_EQ(m_list.Size(), 3u);

    std::vector<int> expected{1, 2, 3};
    EXPECT_EQ(ToVector(m_list), expected);
}

TEST_F(SimpleListTest, PushFrontAndPushBackMixed)
{
    m_list.PushBack(2);
    m_list.PushFront(1);
    m_list.PushBack(3);

    EXPECT_EQ(m_list.Size(), 3u);

    std::vector<int> expected{1, 2, 3};
    EXPECT_EQ(ToVector(m_list), expected);
}

TEST_F(SimpleListTest, InsertBeforeBegin)
{
    m_list.PushBack(2);
    m_list.PushBack(3);

    m_list.Insert(m_list.Begin(), 1);

    EXPECT_EQ(m_list.Size(), 3u);

    std::vector<int> expected{1, 2, 3};
    EXPECT_EQ(ToVector(m_list), expected);
}

TEST_F(SimpleListTest, InsertBeforeMiddlePosition)
{
    m_list.PushBack(1);
    m_list.PushBack(3);

    auto it = m_list.Begin();
    ++it;  // 指向 3

    m_list.Insert(it, 2);

    EXPECT_EQ(m_list.Size(), 3u);

    std::vector<int> expected{1, 2, 3};
    EXPECT_EQ(ToVector(m_list), expected);
}

TEST_F(SimpleListTest, InsertBeforeEndEqualsPushBack)
{
    m_list.PushBack(1);
    m_list.PushBack(2);

    m_list.Insert(m_list.End(), 3);

    EXPECT_EQ(m_list.Size(), 3u);

    std::vector<int> expected{1, 2, 3};
    EXPECT_EQ(ToVector(m_list), expected);
}

TEST_F(SimpleListTest, EraseBegin)
{
    m_list.PushBack(1);
    m_list.PushBack(2);
    m_list.PushBack(3);

    m_list.Erase(m_list.Begin());

    EXPECT_EQ(m_list.Size(), 2u);

    std::vector<int> expected{2, 3};
    EXPECT_EQ(ToVector(m_list), expected);
}

TEST_F(SimpleListTest, EraseMiddle)
{
    m_list.PushBack(1);
    m_list.PushBack(2);
    m_list.PushBack(3);

    auto it = m_list.Begin();
    ++it;  // 指向 2

    m_list.Erase(it);

    EXPECT_EQ(m_list.Size(), 2u);

    std::vector<int> expected{1, 3};
    EXPECT_EQ(ToVector(m_list), expected);
}

TEST_F(SimpleListTest, EraseEndDoesNothing)
{
    m_list.PushBack(1);
    m_list.PushBack(2);

    m_list.Erase(m_list.End());

    EXPECT_EQ(m_list.Size(), 2u);

    std::vector<int> expected{1, 2};
    EXPECT_EQ(ToVector(m_list), expected);
}

TEST_F(SimpleListTest, PopFrontOnEmptyListDoesNothing)
{
    m_list.PopFront();

    EXPECT_EQ(m_list.Size(), 0u);
    EXPECT_EQ(m_list.Begin(), m_list.End());
}

TEST_F(SimpleListTest, PopBackOnEmptyListDoesNothing)
{
    m_list.PopBack();

    EXPECT_EQ(m_list.Size(), 0u);
    EXPECT_EQ(m_list.Begin(), m_list.End());
}

TEST_F(SimpleListTest, PopFrontWorks)
{
    m_list.PushBack(1);
    m_list.PushBack(2);
    m_list.PushBack(3);

    m_list.PopFront();

    EXPECT_EQ(m_list.Size(), 2u);

    std::vector<int> expected{2, 3};
    EXPECT_EQ(ToVector(m_list), expected);
}

TEST_F(SimpleListTest, PopBackWorks)
{
    m_list.PushBack(1);
    m_list.PushBack(2);
    m_list.PushBack(3);

    m_list.PopBack();

    EXPECT_EQ(m_list.Size(), 2u);

    std::vector<int> expected{1, 2};
    EXPECT_EQ(ToVector(m_list), expected);
}

TEST_F(SimpleListTest, PopFrontOnSingleElementList)
{
    m_list.PushBack(1);

    m_list.PopFront();

    EXPECT_EQ(m_list.Size(), 0u);
    EXPECT_EQ(m_list.Begin(), m_list.End());
}

TEST_F(SimpleListTest, PopBackOnSingleElementList)
{
    m_list.PushBack(1);

    m_list.PopBack();

    EXPECT_EQ(m_list.Size(), 0u);
    EXPECT_EQ(m_list.Begin(), m_list.End());
}

TEST_F(SimpleListTest, ClearWorks)
{
    m_list.PushBack(1);
    m_list.PushBack(2);
    m_list.PushBack(3);

    m_list.Clear();

    EXPECT_EQ(m_list.Size(), 0u);
    EXPECT_EQ(m_list.Begin(), m_list.End());
}

TEST_F(SimpleListTest, IteratorTraversalWorks)
{
    m_list.PushBack(10);
    m_list.PushBack(20);
    m_list.PushBack(30);

    auto it = m_list.Begin();
    ASSERT_NE(it, m_list.End());
    EXPECT_EQ(*it, 10);

    ++it;
    ASSERT_NE(it, m_list.End());
    EXPECT_EQ(*it, 20);

    ++it;
    ASSERT_NE(it, m_list.End());
    EXPECT_EQ(*it, 30);

    ++it;
    EXPECT_EQ(it, m_list.End());
}

TEST_F(SimpleListTest, SpliceSingleNodeFromOtherList)
{
    m_list.PushBack(1);
    m_list.PushBack(3);

    SimpleList<int> other;
    other.PushBack(10);
    other.PushBack(20);
    other.PushBack(30);

    auto pos = m_list.Begin();
    ++pos;  // 指向 3

    auto it = other.Begin();
    ++it;  // 指向 20

    m_list.Splice(pos, other, it);

    EXPECT_EQ(m_list.Size(), 3u);
    EXPECT_EQ(other.Size(), 2u);

    std::vector<int> expectedThis{1, 20, 3};
    std::vector<int> expectedOther{10, 30};

    EXPECT_EQ(ToVector(m_list), expectedThis);
    EXPECT_EQ(ToVector(other), expectedOther);
}

TEST_F(SimpleListTest, SpliceNodeToEndFromOtherList)
{
    m_list.PushBack(1);
    m_list.PushBack(2);

    SimpleList<int> other;
    other.PushBack(3);

    m_list.Splice(m_list.End(), other, other.Begin());

    EXPECT_EQ(m_list.Size(), 3u);
    EXPECT_EQ(other.Size(), 0u);

    std::vector<int> expectedThis{1, 2, 3};
    EXPECT_EQ(ToVector(m_list), expectedThis);
    EXPECT_EQ(other.Begin(), other.End());
}

TEST_F(SimpleListTest, SpliceEndIteratorFromOtherDoesNothing)
{
    m_list.PushBack(1);
    m_list.PushBack(2);

    SimpleList<int> other;
    other.PushBack(3);
    other.PushBack(4);

    m_list.Splice(m_list.Begin(), other, other.End());

    EXPECT_EQ(m_list.Size(), 2u);
    EXPECT_EQ(other.Size(), 2u);

    std::vector<int> expectedThis{1, 2};
    std::vector<int> expectedOther{3, 4};

    EXPECT_EQ(ToVector(m_list), expectedThis);
    EXPECT_EQ(ToVector(other), expectedOther);
}

TEST_F(SimpleListTest, SpliceSameListPosEqualsItDoesNothing)
{
    m_list.PushBack(1);
    m_list.PushBack(2);
    m_list.PushBack(3);

    auto it = m_list.Begin();
    ++it;  // 指向 2

    m_list.Splice(it, m_list, it);

    EXPECT_EQ(m_list.Size(), 3u);

    std::vector<int> expected{1, 2, 3};
    EXPECT_EQ(ToVector(m_list), expected);
}

TEST_F(SimpleListTest, SpliceSameListMoveNodeForward)
{
    m_list.PushBack(1);
    m_list.PushBack(2);
    m_list.PushBack(3);
    m_list.PushBack(4);

    auto it = m_list.Begin();
    ++it;  // 指向 2

    auto pos = m_list.End();  // 移到末尾前，相当于放到最后
    m_list.Splice(pos, m_list, it);

    EXPECT_EQ(m_list.Size(), 4u);

    std::vector<int> expected{1, 3, 4, 2};
    EXPECT_EQ(ToVector(m_list), expected);
}

TEST_F(SimpleListTest, SpliceSameListAdjacentNoChange)
{
    m_list.PushBack(1);
    m_list.PushBack(2);
    m_list.PushBack(3);

    auto it = m_list.Begin();
    ++it;  // 指向 2

    auto pos = it;
    ++pos;  // 指向 3，2 本来就在 3 前面

    m_list.Splice(pos, m_list, it);

    EXPECT_EQ(m_list.Size(), 3u);

    std::vector<int> expected{1, 2, 3};
    EXPECT_EQ(ToVector(m_list), expected);
}