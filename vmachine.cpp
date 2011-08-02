/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "vmachine.hpp"
#include "byte_code.hpp"

#include <boost/assert.hpp>

#ifdef _MSC_VER
# pragma warning(disable: 4800) // forcing value to bool 'true' or 'false'
                                // (performance warning)
#endif // _MSC_VER

//!
//! \brief simple virtual machine using a stack pointer and a program counter
//!        The program counter points to the next instruction to execute while
//!        the stack stores our local variables
//!        
//! \precondition Stack pointer points one beyond the last used element
//!               Program counter points to the next instruction to execute
//!
//! \note         If an instruction doesn't pop or push exactly the expected number
//!               of values the stack gets corrupted
//!               
//!

int vmachine::execute(
    std::vector<int> const& code
  , std::vector<int>::const_iterator pc
  , std::vector<int>::iterator frame_ptr
)
{
    std::vector<int>::iterator stack_ptr = frame_ptr;

    while (true)
    {
        BOOST_ASSERT(pc != code.end());

        switch (*pc++)
        {
            case op_neg:
                stack_ptr[-1] = -stack_ptr[-1];
                break;

            case op_not:
                stack_ptr[-1] = !bool(stack_ptr[-1]);
                break;

            case op_add:
                --stack_ptr;
                stack_ptr[-1] += stack_ptr[0];
                break;

            case op_sub:
                --stack_ptr;
                stack_ptr[-1] -= stack_ptr[0];
                break;

            case op_mul:
                --stack_ptr;
                stack_ptr[-1] *= stack_ptr[0];
                break;

            case op_div:
                --stack_ptr;
                stack_ptr[-1] /= stack_ptr[0];
                break;

            case op_eq:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] == stack_ptr[0]);
                break;

            case op_neq:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] != stack_ptr[0]);
                break;

            case op_lt:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] < stack_ptr[0]);
                break;

            case op_lte:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] <= stack_ptr[0]);
                break;

            case op_gt:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] > stack_ptr[0]);
                break;

            case op_gte:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1] >= stack_ptr[0]);
                break;

            case op_and:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1]) && bool(stack_ptr[0]);
                break;

            case op_or:
                --stack_ptr;
                stack_ptr[-1] = bool(stack_ptr[-1]) || bool(stack_ptr[0]);
                break;

            case op_load:
                *stack_ptr++ = frame_ptr[*pc++];
                break;

            case op_store:
                --stack_ptr;
                frame_ptr[*pc++] = stack_ptr[0];
                break;

            case op_int:
                *stack_ptr++ = *pc++;
                break;

            case op_true:
                *stack_ptr++ = true;
                break;

            case op_false:
                *stack_ptr++ = false;
                break;

            case op_jump:
                pc = code.begin() + *pc;
                break;

            case op_jump_if:
                if (!bool(stack_ptr[-1]))
                    pc = code.begin() + *pc;
                else
                    ++pc;
                --stack_ptr;
                break;

            case op_stk_adj:
                stack_ptr += *pc++;
                break;

            case op_call:
                {
                    int nargs = *pc++;
                    int jump = *pc++;

                    // a function call is a recursive call to execute
                    int r = execute(
                        code
                      , code.begin() + jump
                      , stack_ptr - nargs
                    );

                    // cleanup after return from function
                    stack_ptr[-nargs] = r;      //  get return value
                    stack_ptr -= (nargs - 1);   //  the stack will now contain
                                                //  the return value
                }
                break;

            case op_return:
                return stack_ptr[-1];

            case op_print:
              std::cout << *(--stack_ptr);
              break;
            case op_dump:
              std::cout << "dump: "  << *(--stack_ptr) << std::endl;
              break;
            case op_debug:
              std::cout << "debug: "  << *(--stack_ptr) << std::endl;
              break;
        }
    }
    return 0; // TODO(Joerg.Schwaerzler@infineon.com): Implement error handling
              //                                       for this case
}

