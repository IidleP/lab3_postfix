#include "stack.h"
#include <gtest.h>


// Создание
TEST(TStack, can_create_stack_with_positive_length)
{
  ASSERT_NO_THROW(TStack<int> st(5));
}

TEST(TStack, can_create_stack_with_positive_size)
{
    ASSERT_NO_THROW(TStack<int> st(5));
}

TEST(TStack, throws_when_create_stack_with_zero_size)
{
    ASSERT_ANY_THROW(TStack<int> st(0));
}

TEST(TStack, throws_when_create_stack_with_negative_size)
{
    ASSERT_ANY_THROW(TStack<int> st(-5));
}

TEST(TStack, throws_when_create_stack_with_too_large_size)
{
    ASSERT_ANY_THROW(TStack<int> st(MaxStackSize + 1));
}

// empty/full
TEST(TStack, new_stack_is_empty)
{
    TStack<int> st(5);
    EXPECT_TRUE(st.isEmpty());
    EXPECT_FALSE(st.isFull());
    EXPECT_EQ(0, st.GetSize());
}

TEST(TStack, stack_is_full_when_reaches_capacity)
{
    TStack<int> st(3);
    st.Push(1);
    st.Push(2);
    st.Push(3);

    EXPECT_TRUE(st.isFull());
    EXPECT_FALSE(st.isEmpty());
    EXPECT_EQ(3, st.GetSize());
}



// push/pop
TEST(TStack, can_push_element)
{
    TStack<int> st(3);
    ASSERT_NO_THROW(st.Push(10));
    EXPECT_EQ(1, st.GetSize());
}

TEST(TStack, can_pop_element)
{
    TStack<int> st(3);
    st.Push(7);

    int x = 0;
    ASSERT_NO_THROW(x = st.Pop());
    EXPECT_EQ(7, x);
    EXPECT_TRUE(st.isEmpty());
}

TEST(TStack, pop_returns_last_pushed_element)
{
    TStack<int> st(5);
    st.Push(1);
    st.Push(2);
    st.Push(3);

    EXPECT_EQ(3, st.Pop());
    EXPECT_EQ(2, st.Pop());
    EXPECT_EQ(1, st.Pop());
}

TEST(TStack, throws_when_pop_from_empty_stack)
{
    TStack<int> st(5);
    ASSERT_ANY_THROW(st.Pop());
}

TEST(TStack, throws_when_push_to_full_stack)
{
    TStack<int> st(2);
    st.Push(1);
    st.Push(2);

    ASSERT_ANY_THROW(st.Push(3));
}




// top
TEST(TStack, top_returns_last_element_without_removing)
{
    TStack<int> st(3);
    st.Push(4);
    st.Push(9);

    EXPECT_EQ(9, st.Top());
    EXPECT_EQ(2, st.GetSize());
}

TEST(TStack, throws_when_top_from_empty_stack)
{
    TStack<int> st(3);
    ASSERT_ANY_THROW(st.Top());
}




// копирование
TEST(TStack, can_create_copied_stack)
{
    TStack<int> st(3);
    st.Push(1);
    st.Push(2);

    ASSERT_NO_THROW(TStack<int> copy(st));
}

TEST(TStack, copied_stack_is_equal_to_source)
{
    TStack<int> st(3);
    st.Push(1);
    st.Push(2);
    st.Push(3);

    TStack<int> copy(st);

    EXPECT_EQ(3, copy.Pop());
    EXPECT_EQ(2, copy.Pop());
    EXPECT_EQ(1, copy.Pop());
}

TEST(TStack, copied_stack_has_its_own_memory)
{
    TStack<int> st(3);
    st.Push(1);
    st.Push(2);

    TStack<int> copy(st);
    copy.Pop();

    EXPECT_EQ(2, st.GetSize());
    EXPECT_EQ(1, copy.GetSize());
}



// присваивание
TEST(TStack, can_assign_stack_to_itself)
{
    TStack<int> st(3);
    st.Push(1);
    st.Push(2);

    ASSERT_NO_THROW(st = st);
    EXPECT_EQ(2, st.GetSize());
    EXPECT_EQ(2, st.Top());
}

TEST(TStack, can_assign_stacks_of_equal_size)
{
    TStack<int> a(3);
    TStack<int> b(3);

    a.Push(1);
    a.Push(2);
    a.Push(3);

    b = a;

    EXPECT_EQ(3, b.Pop());
    EXPECT_EQ(2, b.Pop());
    EXPECT_EQ(1, b.Pop());
}

TEST(TStack, can_assign_stacks_of_different_size)
{
    TStack<int> a(2);
    TStack<int> b(5);

    a.Push(7);
    a.Push(8);

    b = a;

    EXPECT_EQ(2, b.GetSize());
    EXPECT_EQ(8, b.Pop());
    EXPECT_EQ(7, b.Pop());
}

TEST(TStack, assigned_stack_has_its_own_memory)
{
    TStack<int> a(3);
    TStack<int> b(3);

    a.Push(1);
    b = a;

    b.Pop();

    EXPECT_EQ(1, a.GetSize());
    EXPECT_EQ(0, b.GetSize());
}
