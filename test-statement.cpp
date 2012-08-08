/*=============================================================================
    Copyright (c) 2011 Joerg Schwaerzler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/


//@sa http://www.boost.org/doc/libs/1_46_1/libs/spirit/doc/html/index.html
//@sa http://www.boost.org/doc/libs/1_46_1/


// #define BOOST_TEST_MODULE maxi-c-test-statement

#include "exec.hpp"
#include "statement.hpp"
#include "byte_code.hpp"

#include <vector>
#include <string>

#include <boost/test/unit_test.hpp>


struct StatementFixture
{
  StatementFixture()
  :code_()
  ,testStatementList(code_, testFunctions){};
  ~StatementFixture(){};

  std::vector<int> code_;
  typedef std::string::const_iterator iterator_type;
  typedef statement<iterator_type> TestStatementList;

  // member vars
  boost::spirit::qi::symbols<char, function_info> testFunctions;
  TestStatementList testStatementList;
};

BOOST_FIXTURE_TEST_SUITE(test_statementlist, StatementFixture)

BOOST_AUTO_TEST_CASE(parse_int_i)
{
  std::string script("int32_t i;");
  BOOST_CHECK(testStatementList.variables.find("i") == NULL);
  BOOST_CHECK_NO_THROW(compile(testStatementList, script));
  BOOST_CHECK_EQUAL(code_.size(), 0U);
  BOOST_REQUIRE(testStatementList.variables.find("i") != NULL);
  BOOST_CHECK_EQUAL(testStatementList.variables.find("i")->size, 4);
  BOOST_CHECK_EQUAL(testStatementList.variables.find("i")->sign, true);
  BOOST_CHECK_EQUAL(testStatementList.add_var_2.f.vars.find("i")->id,
      testStatementList.variables.find("i")->id);
}

BOOST_AUTO_TEST_CASE(parse_uint_i)
{
  std::string script("uint32_t i;");
  BOOST_CHECK(testStatementList.variables.find("i") == NULL);
  BOOST_CHECK_NO_THROW(compile(testStatementList, script));
  BOOST_CHECK_EQUAL(code_.size(), 0U);
  BOOST_REQUIRE(testStatementList.variables.find("i") != NULL);
  BOOST_CHECK_EQUAL(testStatementList.variables.find("i")->size, 4);
  BOOST_CHECK_EQUAL(testStatementList.variables.find("i")->sign, false);
  BOOST_CHECK_EQUAL(testStatementList.add_var_2.f.vars.find("i")->id,
      testStatementList.variables.find("i")->id);
}

//BOOST_AUTO_TEST_CASE(parse_int16_t_i)
//{
//  std::string script("int_16_t i;");
//  BOOST_CHECK(testStatementList.variables.find("i") == NULL);
//  BOOST_CHECK_NO_THROW(compile(testStatementList, script));
//  BOOST_CHECK_EQUAL(code_.size(), 0U);
//  BOOST_REQUIRE(testStatementList.variables.find("i") != NULL);
//  BOOST_CHECK_EQUAL(testStatementList.variables.find("i")->size, 4);
//  BOOST_CHECK_EQUAL(testStatementList.variables.find("i")->sign, true);
//  BOOST_CHECK_EQUAL(testStatementList.add_var_2.f.vars.find("i")->id,
//      testStatementList.variables.find("i")->id);
//}
//
//BOOST_AUTO_TEST_CASE(parse_uint_i)
//{
//  std::string script("uint i;");
//  BOOST_CHECK(testStatementList.variables.find("i") == NULL);
//  BOOST_CHECK_NO_THROW(compile(testStatementList, script));
//  BOOST_CHECK_EQUAL(code_.size(), 0U);
//  BOOST_REQUIRE(testStatementList.variables.find("i") != NULL);
//  BOOST_CHECK_EQUAL(testStatementList.variables.find("i")->size, 4);
//  BOOST_CHECK_EQUAL(testStatementList.variables.find("i")->sign, false);
//  BOOST_CHECK_EQUAL(testStatementList.add_var_2.f.vars.find("i")->id,
//      testStatementList.variables.find("i")->id);
//}
//
//BOOST_AUTO_TEST_CASE(parse_int_i)
//{
//  std::string script("int i;");
//  BOOST_CHECK(testStatementList.variables.find("i") == NULL);
//  BOOST_CHECK_NO_THROW(compile(testStatementList, script));
//  BOOST_CHECK_EQUAL(code_.size(), 0U);
//  BOOST_REQUIRE(testStatementList.variables.find("i") != NULL);
//  BOOST_CHECK_EQUAL(testStatementList.variables.find("i")->size, 4);
//  BOOST_CHECK_EQUAL(testStatementList.variables.find("i")->sign, true);
//  BOOST_CHECK_EQUAL(testStatementList.add_var_2.f.vars.find("i")->id,
//      testStatementList.variables.find("i")->id);
//}
//
//BOOST_AUTO_TEST_CASE(parse_uint_i)
//{
//  std::string script("uint i;");
//  BOOST_CHECK(testStatementList.variables.find("i") == NULL);
//  BOOST_CHECK_NO_THROW(compile(testStatementList, script));
//  BOOST_CHECK_EQUAL(code_.size(), 0U);
//  BOOST_REQUIRE(testStatementList.variables.find("i") != NULL);
//  BOOST_CHECK_EQUAL(testStatementList.variables.find("i")->size, 4);
//  BOOST_CHECK_EQUAL(testStatementList.variables.find("i")->sign, false);
//  BOOST_CHECK_EQUAL(testStatementList.add_var_2.f.vars.find("i")->id,
//      testStatementList.variables.find("i")->id);
//}

BOOST_AUTO_TEST_CASE(declare_int_and_init_with_99)
{
  std::string script("int32_t i = 99;");
  BOOST_CHECK_NO_THROW(compile(testStatementList, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 4U);
  BOOST_CHECK_EQUAL(code_[0], op_int);       // have int
  BOOST_CHECK_EQUAL(code_[1], 99);           // load value
  BOOST_CHECK_EQUAL(code_[2], op_store_32);  // store int
  BOOST_CHECK_EQUAL(code_[3], 0);            // store at var pos #0
}

BOOST_AUTO_TEST_CASE(declare_ints_and_init_them_with_1_and_2)
{
  std::string script("int32_t i = 1; int32_t j = 2;");
  BOOST_CHECK_NO_THROW(compile(testStatementList, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 8U);
  BOOST_CHECK_EQUAL(code_[0], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[1], 1);           // load value
  BOOST_CHECK_EQUAL(code_[2], op_store_32); // store int
  BOOST_CHECK_EQUAL(code_[3], 0);           // store at var pos #0
  BOOST_CHECK_EQUAL(code_[4], op_int);      // have int
  BOOST_CHECK_EQUAL(code_[5], 2);           // load value
  BOOST_CHECK_EQUAL(code_[6], op_store_32);    // store int
  BOOST_CHECK_EQUAL(code_[7], 1);           // store at var pos #1
}

BOOST_AUTO_TEST_CASE(parse_if_i_less_than_j_i_equals_1_else_i_equals_2_no_brakets)
{
  std::string script("int32_t i = 1; int32_t j = 2;\
      if (i < j)"
      "  i = 1;"
      "else"
      "  i = 2;");
  BOOST_CHECK_NO_THROW(compile(testStatementList, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 25U);
  std::vector<int>::iterator instruction = code_.begin();
  BOOST_CHECK_EQUAL(*(instruction)++, op_int);      //
  BOOST_CHECK_EQUAL(*(instruction)++, 1);           // value #1
  BOOST_CHECK_EQUAL(*(instruction)++, op_store_32); //
  BOOST_CHECK_EQUAL(*(instruction)++, 0);           // store at var pos #0 == i
  BOOST_CHECK_EQUAL(*(instruction)++, op_int);      //
  BOOST_CHECK_EQUAL(*(instruction)++, 2);           // value #2
  BOOST_CHECK_EQUAL(*(instruction)++, op_store_32); //
  BOOST_CHECK_EQUAL(*(instruction)++, 1);           // store at var pos #1 == j
  BOOST_CHECK_EQUAL(*(instruction)++, op_load_32);
  BOOST_CHECK_EQUAL(*(instruction)++, 0);
  BOOST_CHECK_EQUAL(*(instruction)++, op_load_32);
  BOOST_CHECK_EQUAL(*(instruction)++, 1);
  BOOST_CHECK_EQUAL(*(instruction)++, op_lt);
  BOOST_CHECK_EQUAL(*(instruction)++, op_jump_if);
  BOOST_CHECK_EQUAL(*(instruction)++, 21);          // jump to else path if not lower than
  BOOST_CHECK_EQUAL(*(instruction)++, op_int);      //
  BOOST_CHECK_EQUAL(*(instruction)++, 1);           // value #1
  BOOST_CHECK_EQUAL(*(instruction)++, op_store_32); //
  BOOST_CHECK_EQUAL(*(instruction)++, 0);           // store into i [==index 0]
  BOOST_CHECK_EQUAL(*(instruction)++, op_jump);     //
  BOOST_CHECK_EQUAL(*(instruction)++, 25);          // jump right after the else
  BOOST_CHECK_EQUAL(*(instruction)++, op_int);      //
  BOOST_CHECK_EQUAL(*(instruction)++, 2);           // value #2
  BOOST_CHECK_EQUAL(*(instruction)++, op_store_32); //
  BOOST_CHECK_EQUAL(*(instruction)++, 0);           // store into i [==index 0]
}

BOOST_AUTO_TEST_CASE(parse_if_i_less_than_j_i_equals_1_else_i_equals_2_brakets)
{
  std::string script("int32_t i = 1; int32_t j = 2;\
      if (i < j)"
      "{"
      "  i = 1;"
      "}"
      "else"
      "{"
      "  i = 2;"
      "}");
  BOOST_CHECK_NO_THROW(compile(testStatementList, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 25U);
  std::vector<int>::iterator instruction = code_.begin();
  BOOST_CHECK_EQUAL(*(instruction)++, op_int);      //
  BOOST_CHECK_EQUAL(*(instruction)++, 1);           // value #1
  BOOST_CHECK_EQUAL(*(instruction)++, op_store_32);    //
  BOOST_CHECK_EQUAL(*(instruction)++, 0);           // store at var pos #0 == i
  BOOST_CHECK_EQUAL(*(instruction)++, op_int);      //
  BOOST_CHECK_EQUAL(*(instruction)++, 2);           // value #2
  BOOST_CHECK_EQUAL(*(instruction)++, op_store_32);    //
  BOOST_CHECK_EQUAL(*(instruction)++, 1);           // store at var pos #1 == j
  BOOST_CHECK_EQUAL(*(instruction)++, op_load_32);
  BOOST_CHECK_EQUAL(*(instruction)++, 0);
  BOOST_CHECK_EQUAL(*(instruction)++, op_load_32);
  BOOST_CHECK_EQUAL(*(instruction)++, 1);
  BOOST_CHECK_EQUAL(*(instruction)++, op_lt);
  BOOST_CHECK_EQUAL(*(instruction)++, op_jump_if);
  BOOST_CHECK_EQUAL(*(instruction)++, 21);          // jump to else path if not lower than
  BOOST_CHECK_EQUAL(*(instruction)++, op_int);      //
  BOOST_CHECK_EQUAL(*(instruction)++, 1);           // value #1
  BOOST_CHECK_EQUAL(*(instruction)++, op_store_32);    //
  BOOST_CHECK_EQUAL(*(instruction)++, 0);           // store into i [==index 0]
  BOOST_CHECK_EQUAL(*(instruction)++, op_jump);     //
  BOOST_CHECK_EQUAL(*(instruction)++, 25);          // jump right after the else
  BOOST_CHECK_EQUAL(*(instruction)++, op_int);      //
  BOOST_CHECK_EQUAL(*(instruction)++, 2);           // value #2
  BOOST_CHECK_EQUAL(*(instruction)++, op_store_32);    //
  BOOST_CHECK_EQUAL(*(instruction)++, 0);           // store into i [==index 0]
}

BOOST_AUTO_TEST_CASE(parse_while_i_less_than_j_plusplus_i_no_brakets)
{
  std::string script("int32_t i = 1; int32_t j = 2;\
      while (i < j)"
      "  i = i + 1;");
  BOOST_CHECK_NO_THROW(compile(testStatementList, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 24U);
  std::vector<int>::iterator instruction = code_.begin();
  BOOST_CHECK_EQUAL(*(instruction)++, op_int);      //
  BOOST_CHECK_EQUAL(*(instruction)++, 1);           // value #1
  BOOST_CHECK_EQUAL(*(instruction)++, op_store_32);    //
  BOOST_CHECK_EQUAL(*(instruction)++, 0);           // store at var pos #0 == i
  BOOST_CHECK_EQUAL(*(instruction)++, op_int);      //
  BOOST_CHECK_EQUAL(*(instruction)++, 2);           // value #2
  BOOST_CHECK_EQUAL(*(instruction)++, op_store_32);    //
  BOOST_CHECK_EQUAL(*(instruction)++, 1);           // store at var pos #1 == j
  BOOST_CHECK_EQUAL(*(instruction)++, op_load_32);
  BOOST_CHECK_EQUAL(*(instruction)++, 0);
  BOOST_CHECK_EQUAL(*(instruction)++, op_load_32);
  BOOST_CHECK_EQUAL(*(instruction)++, 1);
  BOOST_CHECK_EQUAL(*(instruction)++, op_lt);
  BOOST_CHECK_EQUAL(*(instruction)++, op_jump_if);
  BOOST_CHECK_EQUAL(*(instruction)++, 24);          // jump to out of the while()
  BOOST_CHECK_EQUAL(*(instruction)++, op_load_32);     //
  BOOST_CHECK_EQUAL(*(instruction)++, 0);           // load i
  BOOST_CHECK_EQUAL(*(instruction)++, op_int);      //
  BOOST_CHECK_EQUAL(*(instruction)++, 1);           // value #1
  BOOST_CHECK_EQUAL(*(instruction)++, op_add);      // add to i
  BOOST_CHECK_EQUAL(*(instruction)++, op_store_32);    //
  BOOST_CHECK_EQUAL(*(instruction)++, 0);           // store into i
  BOOST_CHECK_EQUAL(*(instruction)++, op_jump);     //
  BOOST_CHECK_EQUAL(*(instruction)++, 8);           // loop
}

BOOST_AUTO_TEST_CASE(parse_while_i_less_than_j_plusplus_i_brakets)
{
  std::string script("int32_t i = 1; int32_t j = 2;\
      while (i < j)"
      "{"
      "  i = i + 1;"
      "}");
  BOOST_CHECK_NO_THROW(compile(testStatementList, script));
  BOOST_REQUIRE_EQUAL(code_.size(), 24U);
  std::vector<int>::iterator instruction = code_.begin();
  BOOST_CHECK_EQUAL(*(instruction)++, op_int);      //
  BOOST_CHECK_EQUAL(*(instruction)++, 1);           // value #1
  BOOST_CHECK_EQUAL(*(instruction)++, op_store_32);    //
  BOOST_CHECK_EQUAL(*(instruction)++, 0);           // store at var pos #0 == i
  BOOST_CHECK_EQUAL(*(instruction)++, op_int);      //
  BOOST_CHECK_EQUAL(*(instruction)++, 2);           // value #2
  BOOST_CHECK_EQUAL(*(instruction)++, op_store_32);    //
  BOOST_CHECK_EQUAL(*(instruction)++, 1);           // store at var pos #1 == j
  BOOST_CHECK_EQUAL(*(instruction)++, op_load_32);
  BOOST_CHECK_EQUAL(*(instruction)++, 0);
  BOOST_CHECK_EQUAL(*(instruction)++, op_load_32);
  BOOST_CHECK_EQUAL(*(instruction)++, 1);
  BOOST_CHECK_EQUAL(*(instruction)++, op_lt);
  BOOST_CHECK_EQUAL(*(instruction)++, op_jump_if);
  BOOST_CHECK_EQUAL(*(instruction)++, 24);          // jump to out of the while()
  BOOST_CHECK_EQUAL(*(instruction)++, op_load_32);     //
  BOOST_CHECK_EQUAL(*(instruction)++, 0);           // load i
  BOOST_CHECK_EQUAL(*(instruction)++, op_int);      //
  BOOST_CHECK_EQUAL(*(instruction)++, 1);           // value #1
  BOOST_CHECK_EQUAL(*(instruction)++, op_add);      // add to i
  BOOST_CHECK_EQUAL(*(instruction)++, op_store_32);    //
  BOOST_CHECK_EQUAL(*(instruction)++, 0);           // store into i
  BOOST_CHECK_EQUAL(*(instruction)++, op_jump);     //
  BOOST_CHECK_EQUAL(*(instruction)++, 8);           // loop
}

BOOST_AUTO_TEST_SUITE_END()

