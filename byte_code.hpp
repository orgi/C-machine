/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BYTE_CODE_HPP_
#define BYTE_CODE_HPP_

///////////////////////////////////////////////////////////////////////////////
//  The Virtual Machine
///////////////////////////////////////////////////////////////////////////////
enum byte_code
{
    op_neg,         //  negate the top stack entry
    op_add,         //  add top two stack entries
    op_sub,         //  subtract top two stack entries
    op_mul,         //  multiply top two stack entries
    op_div,         //  divide top two stack entries
// 5:
    op_not,         //  boolean negate the top stack entry
    op_eq,          //  compare the top two stack entries for ==
    op_neq,         //  compare the top two stack entries for !=
    op_lt,          //  compare the top two stack entries for <
    op_lte,         //  compare the top two stack entries for <=
    op_gt,          //  compare the top two stack entries for >
    op_gte,         //  compare the top two stack entries for >=
// 12:
    op_and,         //  logical and top two stack entries
    op_or,          //  logical or top two stack entries
// 14:
    op_load,        //  load a 32-bit variable
    op_store,       //  store a 32-bit variable
// 16:
    op_int,         //  push constant integer into the stack
    op_true,        //  push constant 0 into the stack
    op_false,       //  push constant 1 into the stack
// 19:
    op_jump_if,     //  jump to an absolute position in the code if top stack
                    //  evaluates to false
    op_jump,        //  jump to an absolute position in the code
// 21:
    op_stk_adj,     // adjust the stack (for args and locals)
    op_call,        // function call
    op_return,      // return from function
// 24:
    op_print,       // print to console
    op_dump,        // dump to console
    op_debug,       // debug to console ;)
// 26:
    op_short		// push constant short to stack
};



#endif /* BYTE_CODE_HPP_ */
