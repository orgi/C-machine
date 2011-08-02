/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef VMACHINE_HPP_
#define VMACHINE_HPP_

#include <vector>

//!
//! The virtual machine...
//!
class vmachine
{
public:

    vmachine(unsigned stackSize = 4096)
      : stack(stackSize)
    {
    }

    int execute(
        std::vector<int> const& code            // the program code
      , std::vector<int>::const_iterator pc     // program counter
      , std::vector<int>::iterator frame_ptr    // start of arguments and locals
    );

    std::vector<int> stack;
};


#endif /* VMACHINE_HPP_ */
