/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef FUNCTION_STATE_RESET_HPP_
#define FUNCTION_STATE_RESET_HPP_

// boost
#include <boost/spirit/include/qi.hpp>
// STL
#include <vector>

///////////////////////////////////////////////////////////////////////////////
//  A functor that resets the function-related state variables
///////////////////////////////////////////////////////////////////////////////
struct function_state_reset
{
    template <typename>
    struct result { typedef void type; };

    function_state_reset(
        std::vector<int>& code
      , boost::spirit::qi::symbols<char, int>& vars
      , int& nvars)
      : code(code)
      , vars(vars)
      , nvars(nvars)
    {
    }

    void operator()(int address) const
    {
        code[address+1] = nvars;
        nvars = 0; // reset
        vars.clear(); // reset
    };

    std::vector<int>& code;
    boost::spirit::qi::symbols<char, int>& vars;
    int& nvars;
};



#endif /* FUNCTION_STATE_RESET_HPP_ */
