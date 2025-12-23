#include "postfix.h"
#include <gtest.h>


//Создание 
TEST(TPostfix, can_create_empty_postfix)
{
  ASSERT_NO_THROW(TPostfix p);
}

TEST(TPostfix, can_create_postfix_with_correct_expression)
{
    ASSERT_NO_THROW(TPostfix p("1+2"));
}

TEST(TPostfix, throws_on_empty_expression)
{
    ASSERT_ANY_THROW(TPostfix p(""));
}


//Валидация
TEST(TPostfix, validate_correct_expression)
{
    TPostfix p("1+2*3");
    EXPECT_TRUE(p.Validate());
}

TEST(TPostfix, validate_expression_with_spaces)
{
    TPostfix p(" ( 1 + 2 ) * 3 ");
    EXPECT_TRUE(p.Validate());
}

TEST(TPostfix, validate_expression_with_unary_minus)
{
    TPostfix p("-3+2");
    EXPECT_TRUE(p.Validate());
}



//Лексемы
TEST(TPostfix, correct_lexems_for_simple_expression)
{
    TPostfix p("1+2");

    vector<string> lex = p.GetLexems();
    ASSERT_EQ(3, lex.size());

    EXPECT_EQ("1", lex[0]);
    EXPECT_EQ("+", lex[1]);
    EXPECT_EQ("2", lex[2]);
}

TEST(TPostfix, correct_lexems_for_unary_minus)
{
    TPostfix p("-5");

    vector<string> lex = p.GetLexems();
    ASSERT_EQ(2, lex.size());

    EXPECT_EQ("~", lex[0]);
    EXPECT_EQ("5", lex[1]);
}


//Постикс
TEST(TPostfix, correct_postfix_for_simple_expression)
{
    TPostfix p("1+2");
    EXPECT_EQ("1 2 +", p.GetPostfix());
}

TEST(TPostfix, correct_postfix_with_priority)
{
    TPostfix p("1+2*3");
    EXPECT_EQ("1 2 3 * +", p.GetPostfix());
}

TEST(TPostfix, correct_postfix_with_parentheses)
{
    TPostfix p("(1+2)*3");
    EXPECT_EQ("1 2 + 3 *", p.GetPostfix());
}

TEST(TPostfix, correct_postfix_with_unary_minus)
{
    TPostfix p("-3*4");
    EXPECT_EQ("3 ~ 4 *", p.GetPostfix());
}

TEST(TPostfix, correct_postfix_with_double_unary_minus)
{
    TPostfix p("--5");
    EXPECT_EQ("5 ~ ~", p.GetPostfix());
}



// Вычисления
TEST(TPostfix, can_calculate_simple_expression)
{
    TPostfix p("1+2");
    EXPECT_EQ(3, p.Calculate());
}

TEST(TPostfix, can_calculate_expression_with_priority)
{
    TPostfix p("1+2*3");
    EXPECT_EQ(7, p.Calculate());
}

TEST(TPostfix, can_calculate_expression_with_parentheses)
{
    TPostfix p("(1+2)*3");
    EXPECT_EQ(9, p.Calculate());
}

TEST(TPostfix, can_calculate_expression_with_unary_minus)
{
    TPostfix p("-3*4");
    EXPECT_EQ(-12, p.Calculate());
}

TEST(TPostfix, can_calculate_double_unary_minus)
{
    TPostfix p("--5");
    EXPECT_EQ(5, p.Calculate());
}

TEST(TPostfix, can_calculate_complex_expression)
{
    TPostfix p("-(3+4)*2/(1-5)");
    EXPECT_DOUBLE_EQ(3.5, p.Calculate());
}


// Дроби
TEST(TPostfix, can_calculate_fractional_numbers)
{
    TPostfix p("1.5+2.5");
    EXPECT_DOUBLE_EQ(4.0, p.Calculate());
}

TEST(TPostfix, can_calculate_dot_fraction)
{
    TPostfix p("0.5+1");
    EXPECT_DOUBLE_EQ(1.5, p.Calculate());
}


// Ошибки, исключения
TEST(TPostfix, throws_on_invalid_character)
{
    ASSERT_ANY_THROW(TPostfix p("1+a"));
}

TEST(TPostfix, throws_on_invalid_symbol)
{
    ASSERT_ANY_THROW(TPostfix p("1$2"));
}

TEST(TPostfix, throws_on_unbalanced_parentheses)
{
    ASSERT_ANY_THROW(TPostfix p("(1+2"));
}

TEST(TPostfix, throws_on_syntax_error)
{
    ASSERT_ANY_THROW(TPostfix p("1+*2"));
}

TEST(TPostfix, throws_on_division_by_zero)
{
    TPostfix p("1/0");
    ASSERT_ANY_THROW(p.Calculate());
}

TEST(TPostfix, throws_on_incomplete_expression)
{
    ASSERT_ANY_THROW(TPostfix p("1/"));
}


