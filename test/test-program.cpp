/*=============================================================================
    Copyright (c) 2011 Joerg Schwaerzler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/


//@sa http://www.boost.org/doc/libs/1_46_1/libs/spirit/doc/html/index.html


#define BOOST_TEST_MODULE C-machine-test

#include "exec.hpp"
#include "program.hpp"
#include "byte_code.hpp"

#include <vector>
#include <string>

#include <boost/test/unit_test.hpp>

struct ProgramFixture
{
  ProgramFixture()
  :code_()
  ,testProgram(code_){};
  ~ProgramFixture(){};

  std::vector<int> code_;
  typedef std::string::const_iterator iterator_type;
  typedef program<iterator_type> TestProgram;

  TestProgram testProgram;
};

BOOST_FIXTURE_TEST_SUITE(test_program, ProgramFixture)

BOOST_AUTO_TEST_CASE(parse_void_func_void_declaration)
{
  std::string script("void func(){int32_t i;}");
  BOOST_CHECK_NO_THROW(compile(testProgram, script));

  BOOST_CHECK(testProgram.functions.find("func") != NULL);
  BOOST_REQUIRE_EQUAL(code_.size(), 2U);
  BOOST_CHECK_EQUAL(code_[0], op_stk_adj);  // adjust stack for function arguments
  BOOST_CHECK_EQUAL(code_[1], 1);           // int i ... == Single variable on stack
}

BOOST_AUTO_TEST_CASE(parse_int_func_returning_sum_of_both_arguments)
{
  std::string script("int func(arg1, arg2){return arg1 + arg2;}");
  BOOST_CHECK_NO_THROW(compile(testProgram, script));

  BOOST_CHECK(testProgram.functions.find("func") != NULL);
  BOOST_REQUIRE_EQUAL(code_.size(), 8U);
  BOOST_CHECK_EQUAL(code_[0], op_stk_adj);  // adjust stack for function arguments
  BOOST_CHECK_EQUAL(code_[1], 2);           //
  BOOST_CHECK_EQUAL(code_[2], op_load_32);  //
  BOOST_CHECK_EQUAL(code_[3], 0);           // TODO: currently I'm unsure why the values are loaded...
  BOOST_CHECK_EQUAL(code_[4], op_load_32);  //
  BOOST_CHECK_EQUAL(code_[5], 1);           //
  BOOST_CHECK_EQUAL(code_[6], op_add);
  BOOST_CHECK_EQUAL(code_[7], op_return);   // return arg
}

BOOST_AUTO_TEST_CASE(execute_one_add_two_gives_three)
{
  std::string script(
      "int main(argc, argv){\n"\
      "int32_t one = 1;\n"\
      "int32_t two = 2;\n"\
      "two = 3;\n"\
      "return 0;\n"\
      "}"
      );

  BOOST_CHECK_EQUAL(execute(script), 0);
  BOOST_CHECK_NO_THROW(execute(script));
}

BOOST_AUTO_TEST_SUITE_END()
