/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef COMPILE_OP_HPP_
#define COMPILE_OP_HPP_

#include "byte_code.hpp"

#include <boost/spirit/include/qi.hpp>
#include <vector>

struct compile_op
{
    template <typename A,
      typename B = boost::spirit::qi::unused_type,
      typename C = boost::spirit::qi::unused_type>
    struct result { typedef void type; };

    compile_op(std::vector<int>& code)
      : code(code)
    {
    }

    void operator()(int a) const
    {
        code.push_back(a);
    }

    void operator()(int a, int b) const
    {
        code.push_back(a);
        code.push_back(b);
    }

    void operator()(int a, int b, int c) const
    {
        code.push_back(a);
        code.push_back(b);
        code.push_back(c);
    }

    // special overload for function calls
    void operator()(function_info const& info, int got_nargs, bool& parse_result) const
    {
        if (got_nargs == info.arity)
        {
            code.push_back(op_call);
            code.push_back(info.arity);
            code.push_back(info.address);
        }
        else
        {
            parse_result = false; // fail the parse
            std::cerr << "wrong number of arguments" << std::endl;
        }
    }

    std::vector<int>& code;
};



#endif /* COMPILE_OP_HPP_ */
