/*=============================================================================
    Copyright (c) 2011 Joerg Schwaerzler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "exec.hpp"
#include "expression.hpp"
#include "byte_code.hpp"

#include <vector>
#include <string>

#include <boost/test/unit_test.hpp>


struct ExpressionFixture
{
  ExpressionFixture()
  :code_()
  ,testVariables()
  ,testFunctions()
  ,testExpression(code_, testVariables, testFunctions)
  {
    const std::string i = "i";
    const std::string j = "j";
    testVariables.add(i.begin(), i.end(), 0);
    testVariables.add(j.begin(), j.end(), 0);
  };
  ~ExpressionFixture(){};

  std::vector<int> code_;
  typedef std::string::const_iterator iterator_type;
  typedef expression<iterator_type> TestExpression;

  // member vars
  boost::spirit::qi::symbols<char, int> testVariables;
  boost::spirit::qi::symbols<char, function_info> testFunctions;
  TestExpression testExpression;
};

BOOST_FIXTURE_TEST_SUITE(test_statementlist, ExpressionFixture)

BOOST_AUTO_TEST_CASE(parse_1_plus_2)
{
  std::string script("1 + 2;");
  BOOST_CHECK_NO_THROW(compile(testExpression, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 5U);
  BOOST_CHECK_EQUAL(code_[0], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[1], 1);           // load value
  BOOST_CHECK_EQUAL(code_[2], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[3], 2);           // load value
  BOOST_CHECK_EQUAL(code_[4], op_add);      // add values
}

BOOST_AUTO_TEST_CASE(parse_3_minus_2)
{
  std::string script("3 - 2;");
  BOOST_CHECK_NO_THROW(compile(testExpression, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 5U);
  BOOST_CHECK_EQUAL(code_[0], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[1], 3);           // load value
  BOOST_CHECK_EQUAL(code_[2], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[3], 2);           // load value
  BOOST_CHECK_EQUAL(code_[4], op_sub);
}

BOOST_AUTO_TEST_CASE(parse_3_times_2)
{
  std::string script("3 * 2;");
  BOOST_CHECK_NO_THROW(compile(testExpression, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 5U);
  BOOST_CHECK_EQUAL(code_[0], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[1], 3);           // load value
  BOOST_CHECK_EQUAL(code_[2], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[3], 2);           // load value
  BOOST_CHECK_EQUAL(code_[4], op_mul);
}

BOOST_AUTO_TEST_CASE(parse_3_divided_by_2)
{
  std::string script("3 / 2;");
  BOOST_CHECK_NO_THROW(compile(testExpression, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 5U);
  BOOST_CHECK_EQUAL(code_[0], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[1], 3);           // load value
  BOOST_CHECK_EQUAL(code_[2], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[3], 2);           // load value
  BOOST_CHECK_EQUAL(code_[4], op_div);
}

//! not yet available
BOOST_AUTO_TEST_CASE(parse_i_negate)
{
  std::string script("-i;");
  BOOST_CHECK_NO_THROW(compile(testExpression, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 3U);
  BOOST_CHECK_EQUAL(code_[0], op_load);     // load variable
  BOOST_CHECK_EQUAL(code_[1], 0);           // variable #0
  BOOST_CHECK_EQUAL(code_[2], op_neg);      // negate
}
BOOST_AUTO_TEST_CASE(parse_not_i)
{
  std::string script("!i;");
  BOOST_CHECK_NO_THROW(compile(testExpression, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 3U);
  BOOST_CHECK_EQUAL(code_[0], op_load);     // load int
  BOOST_CHECK_EQUAL(code_[1], 0);           // variable #0
  BOOST_CHECK_EQUAL(code_[2], op_not);      // negate
}


//op_eq,          //  compare the top two stack entries for ==
BOOST_AUTO_TEST_CASE(parse_3_equal_to_2)
{
  std::string script("3 == 2;");
  BOOST_CHECK_NO_THROW(compile(testExpression, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 5U);
  BOOST_CHECK_EQUAL(code_[0], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[1], 3);           // load value
  BOOST_CHECK_EQUAL(code_[2], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[3], 2);           // load value
  BOOST_CHECK_EQUAL(code_[4], op_eq);
}
//op_neq,         //  compare the top two stack entries for !=
BOOST_AUTO_TEST_CASE(parse_3_not_equal_two_2)
{
  std::string script("3 != 2;");
  BOOST_CHECK_NO_THROW(compile(testExpression, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 5U);
  BOOST_CHECK_EQUAL(code_[0], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[1], 3);           // load value
  BOOST_CHECK_EQUAL(code_[2], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[3], 2);           // load value
  BOOST_CHECK_EQUAL(code_[4], op_neq);
}
//op_lt,          //  compare the top two stack entries for <
BOOST_AUTO_TEST_CASE(parse_3_lower_than_2)
{
  std::string script("3 < 2;");
  BOOST_CHECK_NO_THROW(compile(testExpression, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 5U);
  BOOST_CHECK_EQUAL(code_[0], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[1], 3);           // load value
  BOOST_CHECK_EQUAL(code_[2], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[3], 2);           // load value
  BOOST_CHECK_EQUAL(code_[4], op_lt);
}
//op_lte,         //  compare the top two stack entries for <=
BOOST_AUTO_TEST_CASE(parse_3_less_or_equal_2)
{
  std::string script("3 <= 2;");
  BOOST_CHECK_NO_THROW(compile(testExpression, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 5U);
  BOOST_CHECK_EQUAL(code_[0], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[1], 3);           // load value
  BOOST_CHECK_EQUAL(code_[2], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[3], 2);           // load value
  BOOST_CHECK_EQUAL(code_[4], op_lte);
}
//op_gt,          //  compare the top two stack entries for >
BOOST_AUTO_TEST_CASE(parse_3_greater_than_2)
{
  std::string script("3 > 2;");
  BOOST_CHECK_NO_THROW(compile(testExpression, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 5U);
  BOOST_CHECK_EQUAL(code_[0], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[1], 3);           // load value
  BOOST_CHECK_EQUAL(code_[2], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[3], 2);           // load value
  BOOST_CHECK_EQUAL(code_[4], op_gt);
}
//op_gte,         //  compare the top two stack entries for >=
BOOST_AUTO_TEST_CASE(parse_greater_or_qual_2)
{
  std::string script("3 >= 2;");
  BOOST_CHECK_NO_THROW(compile(testExpression, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 5U);
  BOOST_CHECK_EQUAL(code_[0], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[1], 3);           // load value
  BOOST_CHECK_EQUAL(code_[2], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[3], 2);           // load value
  BOOST_CHECK_EQUAL(code_[4], op_gte);
}
//
//op_and,         //  logical and top two stack entries
BOOST_AUTO_TEST_CASE(parse_3_and_2)
{
  std::string script("3 && 2;");
  BOOST_CHECK_NO_THROW(compile(testExpression, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 5U);
  BOOST_CHECK_EQUAL(code_[0], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[1], 3);           // load value
  BOOST_CHECK_EQUAL(code_[2], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[3], 2);           // load value
  BOOST_CHECK_EQUAL(code_[4], op_and);
}
//op_or,          //  logical or top two stack entries
BOOST_AUTO_TEST_CASE(parse_3_or_2)
{
  std::string script("3 || 2;");
  BOOST_CHECK_NO_THROW(compile(testExpression, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 5U);
  BOOST_CHECK_EQUAL(code_[0], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[1], 3);           // load value
  BOOST_CHECK_EQUAL(code_[2], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[3], 2);           // load value
  BOOST_CHECK_EQUAL(code_[4], op_or);
}

BOOST_AUTO_TEST_CASE(parse_3_times_2_plus_1)
{
  std::string script("3 * 2 + 1;");
  BOOST_CHECK_NO_THROW(compile(testExpression, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 8U);
  BOOST_CHECK_EQUAL(code_[0], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[1], 3);           // load value
  BOOST_CHECK_EQUAL(code_[2], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[3], 2);           // load value
  BOOST_CHECK_EQUAL(code_[4], op_mul);
  BOOST_CHECK_EQUAL(code_[5], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[6], 1);           // load value
  BOOST_CHECK_EQUAL(code_[7], op_add);
}

BOOST_AUTO_TEST_CASE(parse_3_times_left_bracket_2_plus_1_right_bracket)
{
  std::string script("3 * (2 + 1);");
  BOOST_CHECK_NO_THROW(compile(testExpression, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 8U);
  BOOST_CHECK_EQUAL(code_[0], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[1], 3);           // load value
  BOOST_CHECK_EQUAL(code_[2], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[3], 2);           // load value
  BOOST_CHECK_EQUAL(code_[4], op_int);
  BOOST_CHECK_EQUAL(code_[5], 1);
  BOOST_CHECK_EQUAL(code_[6], op_add);
  BOOST_CHECK_EQUAL(code_[7], op_mul);
}

BOOST_AUTO_TEST_CASE(parse_3_divided_by_2_minus_1)
{
  std::string script("3 / 2 - 1;");
  BOOST_CHECK_NO_THROW(compile(testExpression, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 8U);
  BOOST_CHECK_EQUAL(code_[0], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[1], 3);           // load value
  BOOST_CHECK_EQUAL(code_[2], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[3], 2);           // load value
  BOOST_CHECK_EQUAL(code_[4], op_div);
  BOOST_CHECK_EQUAL(code_[5], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[6], 1);           // load value
  BOOST_CHECK_EQUAL(code_[7], op_sub);
}

BOOST_AUTO_TEST_CASE(parse_3_divided_by_left_bracket_2_minus_1_right_bracket)
{
  std::string script("3 / (2 - 1);");
  BOOST_CHECK_NO_THROW(compile(testExpression, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 8U);
  BOOST_CHECK_EQUAL(code_[0], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[1], 3);           // load value
  BOOST_CHECK_EQUAL(code_[2], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[3], 2);           // load value
  BOOST_CHECK_EQUAL(code_[4], op_int);
  BOOST_CHECK_EQUAL(code_[5], 1);
  BOOST_CHECK_EQUAL(code_[6], op_sub);
  BOOST_CHECK_EQUAL(code_[7], op_div);
}

BOOST_AUTO_TEST_CASE(parse_minus_3_divided_by_minus_left_bracket_2_minus_1_right_bracket)
{
  std::string script("-3 / -(2 - 1);");
  BOOST_CHECK_NO_THROW(compile(testExpression, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 10U);
  BOOST_CHECK_EQUAL(code_[0], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[1], 3);           // load value
  BOOST_CHECK_EQUAL(code_[2], op_neg);
  BOOST_CHECK_EQUAL(code_[3], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[4], 2);           // load value
  BOOST_CHECK_EQUAL(code_[5], op_int);
  BOOST_CHECK_EQUAL(code_[6], 1);
  BOOST_CHECK_EQUAL(code_[7], op_sub);
  BOOST_CHECK_EQUAL(code_[8], op_neg);
  BOOST_CHECK_EQUAL(code_[9], op_div);
}

BOOST_AUTO_TEST_SUITE_END()


