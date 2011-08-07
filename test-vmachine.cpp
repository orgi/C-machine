/*=============================================================================
    Copyright (c) 2011 Joerg Schwaerzler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/


//@sa http://www.boost.org/doc/libs/1_46_1/libs/spirit/doc/html/index.html
//@sa http://www.boost.org/doc/libs/1_46_1/

#include "vmachine.hpp"
#include "byte_code.hpp"

#include <boost/test/unit_test.hpp>


struct VmachineFixture
{
  VmachineFixture()
  :code_() // should really be enough for the tests
  ,pc_()
  ,frame_ptr_()
  ,testMachine_(){code_.reserve(50);};
  ~VmachineFixture(){};

  std::vector<int>                 code_;        // the program code
  std::vector<int>::const_iterator pc_;          // program counter
  std::vector<int>::iterator       frame_ptr_;

  vmachine                         testMachine_;
};

BOOST_FIXTURE_TEST_SUITE(test_vmachine_stack_operations, VmachineFixture)
BOOST_AUTO_TEST_CASE(push_three_to_stack_gives_three)
{
  code_.push_back(op_int);
  code_.push_back(3);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(3, retval);
  BOOST_CHECK_EQUAL(3, testMachine_.stack[0]);
}

BOOST_AUTO_TEST_CASE(push_true_to_stack_gives_one)
{
  code_.push_back(op_true);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(1, retval);
  BOOST_CHECK_EQUAL(1, testMachine_.stack[0]);
  BOOST_CHECK_EQUAL(true, retval);
  BOOST_CHECK_EQUAL(true, testMachine_.stack[0]);
}

BOOST_AUTO_TEST_CASE(push_false_to_stack_gives_zero)
{
  code_.push_back(op_false);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(0, retval);
  BOOST_CHECK_EQUAL(0, testMachine_.stack[0]);
  BOOST_CHECK_EQUAL(false, retval);
  BOOST_CHECK_EQUAL(false, testMachine_.stack[0]);
}


BOOST_AUTO_TEST_CASE(load_three_from_stack_at_eleven_gives_three)
{
  code_.push_back(op_load);
  code_.push_back(11);
  code_.push_back(op_return);

  testMachine_.stack[11] = 3;

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(3, retval);
  BOOST_CHECK_EQUAL(3, testMachine_.stack[0]);
}
BOOST_AUTO_TEST_CASE(store_six_to_stack_at_twelfe_gives_six)
{
  code_.push_back(op_store);
  code_.push_back(12);
  code_.push_back(op_return);

  testMachine_.stack[0] = 33; // our return value
  testMachine_.stack[1] = 6;  // value to store

  int retval = 0;
  BOOST_CHECK_EQUAL(0, testMachine_.stack[12 + 2]);
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin() + 2)); // +2 is required to satisfy
                                                           // both the return and the store

//  BOOST_CHECK_EQUAL(6, retval);
  BOOST_CHECK_EQUAL(33, testMachine_.stack[0]);
  BOOST_CHECK_EQUAL(6, testMachine_.stack[12 + 2]);
}
BOOST_AUTO_TEST_SUITE_END();


BOOST_FIXTURE_TEST_SUITE(test_vmachine_operators, VmachineFixture)

BOOST_AUTO_TEST_CASE(negate_2_gives_minus_2)
{
  code_.push_back(op_int);
  code_.push_back(2);
  code_.push_back(op_neg);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(-2, retval);
  BOOST_CHECK_EQUAL(-2, testMachine_.stack[0]);
}

BOOST_AUTO_TEST_CASE(two_plus_1_gives_three)
{
  code_.push_back(op_int);
  code_.push_back(2);
  code_.push_back(op_int);
  code_.push_back(1);
  code_.push_back(op_add);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(3, retval);
  BOOST_CHECK_EQUAL(3, testMachine_.stack[0]);
}

BOOST_AUTO_TEST_CASE(two_minus_1_gives_one)
{
  code_.push_back(op_int);
  code_.push_back(2);
  code_.push_back(op_int);
  code_.push_back(1);
  code_.push_back(op_sub);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(1, retval);
  BOOST_CHECK_EQUAL(1, testMachine_.stack[0]);
}

BOOST_AUTO_TEST_CASE(two_times_3_gives_6)
{
  code_.push_back(op_int);
  code_.push_back(2);
  code_.push_back(op_int);
  code_.push_back(3);
  code_.push_back(op_mul);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(6, retval);
  BOOST_CHECK_EQUAL(6, testMachine_.stack[0]);
}

BOOST_AUTO_TEST_CASE(nine_divided_by_3_gives_3)
{
  code_.push_back(op_int);
  code_.push_back(9);
  code_.push_back(op_int);
  code_.push_back(3);
  code_.push_back(op_div);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(3, retval);
  BOOST_CHECK_EQUAL(3, testMachine_.stack[0]);
}

BOOST_AUTO_TEST_CASE(not_zero_gives_true)
{
  code_.push_back(op_int);
  code_.push_back(0);
  code_.push_back(op_not);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(true, retval);
  BOOST_CHECK_EQUAL(true, testMachine_.stack[0]);
}

BOOST_AUTO_TEST_CASE(not_five_gives_false)
{
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_not);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(false, retval);
  BOOST_CHECK_EQUAL(false, testMachine_.stack[0]);
}

BOOST_AUTO_TEST_CASE(five_equal_five_is_true)
{
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_eq);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(true, retval);
  BOOST_CHECK_EQUAL(true, testMachine_.stack[0]);
}

BOOST_AUTO_TEST_CASE(five_equal_three_is_false)
{
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_int);
  code_.push_back(3);
  code_.push_back(op_eq);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(false, retval);
  BOOST_CHECK_EQUAL(false, testMachine_.stack[0]);
}

BOOST_AUTO_TEST_CASE(five_not_equal_five_is_false)
{
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_neq);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(false, retval);
  BOOST_CHECK_EQUAL(false, testMachine_.stack[0]);
}

BOOST_AUTO_TEST_CASE(five_not_equal_three_is_true)
{
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_int);
  code_.push_back(3);
  code_.push_back(op_neq);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(true, retval);
  BOOST_CHECK_EQUAL(true, testMachine_.stack[0]);
}


BOOST_AUTO_TEST_CASE(five_lower_than_five_is_false)
{
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_lt);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(false, retval);
  BOOST_CHECK_EQUAL(false, testMachine_.stack[0]);
}

BOOST_AUTO_TEST_CASE(five_lower_than_three_is_false)
{
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_int);
  code_.push_back(3);
  code_.push_back(op_lt);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(false, retval);
  BOOST_CHECK_EQUAL(false, testMachine_.stack[0]);
}


BOOST_AUTO_TEST_CASE(three_lower_than_five_is_true)
{
  code_.push_back(op_int);
  code_.push_back(3);
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_lt);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(true, retval);
  BOOST_CHECK_EQUAL(true, testMachine_.stack[0]);
}

BOOST_AUTO_TEST_CASE(five_lower_than_or_equal_five_is_true)
{
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_lte);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(true, retval);
  BOOST_CHECK_EQUAL(true, testMachine_.stack[0]);
}

BOOST_AUTO_TEST_CASE(five_lower_than_or_equal_three_is_false)
{
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_int);
  code_.push_back(3);
  code_.push_back(op_lte);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(false, retval);
  BOOST_CHECK_EQUAL(false, testMachine_.stack[0]);
}


BOOST_AUTO_TEST_CASE(three_lower_than_or_equal_five_is_true)
{
  code_.push_back(op_int);
  code_.push_back(3);
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_lte);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(true, retval);
  BOOST_CHECK_EQUAL(true, testMachine_.stack[0]);
}

BOOST_AUTO_TEST_CASE(five_greater_than_five_is_false)
{
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_gt);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(false, retval);
  BOOST_CHECK_EQUAL(false, testMachine_.stack[0]);
}

BOOST_AUTO_TEST_CASE(five_greater_than_three_is_true)
{
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_int);
  code_.push_back(3);
  code_.push_back(op_gt);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(true, retval);
  BOOST_CHECK_EQUAL(true, testMachine_.stack[0]);
}


BOOST_AUTO_TEST_CASE(three_greater_than_five_is_false)
{
  code_.push_back(op_int);
  code_.push_back(3);
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_gt);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(false, retval);
  BOOST_CHECK_EQUAL(false, testMachine_.stack[0]);
}

BOOST_AUTO_TEST_CASE(five_greater_than_or_equal_five_is_true)
{
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_gte);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(true, retval);
  BOOST_CHECK_EQUAL(true, testMachine_.stack[0]);
}

BOOST_AUTO_TEST_CASE(five_greater_than_or_equal_three_is_true)
{
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_int);
  code_.push_back(3);
  code_.push_back(op_gte);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(true, retval);
  BOOST_CHECK_EQUAL(true, testMachine_.stack[0]);
}


BOOST_AUTO_TEST_CASE(three_greater_than_or_equal_five_is_false)
{
  code_.push_back(op_int);
  code_.push_back(3);
  code_.push_back(op_int);
  code_.push_back(5);
  code_.push_back(op_gte);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(false, retval);
  BOOST_CHECK_EQUAL(false, testMachine_.stack[0]);
}

BOOST_AUTO_TEST_CASE(two_AND_one_gives_true)
{
  code_.push_back(op_int);
  code_.push_back(2);
  code_.push_back(op_int);
  code_.push_back(1);
  code_.push_back(op_and);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(true, retval);
  BOOST_CHECK_EQUAL(true, testMachine_.stack[0]);
}
BOOST_AUTO_TEST_CASE(two_AND_zero_gives_false)
{
  code_.push_back(op_int);
  code_.push_back(2);
  code_.push_back(op_int);
  code_.push_back(0);
  code_.push_back(op_and);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(false, retval);
  BOOST_CHECK_EQUAL(false, testMachine_.stack[0]);
}
BOOST_AUTO_TEST_CASE(zero_AND_one_gives_false)
{
  code_.push_back(op_int);
  code_.push_back(0);
  code_.push_back(op_int);
  code_.push_back(1);
  code_.push_back(op_and);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(false, retval);
  BOOST_CHECK_EQUAL(false, testMachine_.stack[0]);
}
BOOST_AUTO_TEST_CASE(zero_AND_zero_gives_false)
{
  code_.push_back(op_int);
  code_.push_back(0);
  code_.push_back(op_int);
  code_.push_back(0);
  code_.push_back(op_and);
  code_.push_back(op_return);

  int retval = 0;
  BOOST_REQUIRE_NO_THROW(retval = testMachine_.execute(code_,
                         code_.begin(),
                         testMachine_.stack.begin()));

  BOOST_CHECK_EQUAL(false, retval);
  BOOST_CHECK_EQUAL(false, testMachine_.stack[0]);
}
BOOST_AUTO_TEST_SUITE_END();


