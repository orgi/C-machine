/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef PROGRAM_HPP_
#define PROGRAM_HPP_

#include "white_space.hpp"
#include "statement.hpp"
#include "compile_op.hpp"
#include "function_adder.hpp"
#include "function_state_reset.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>

#include <vector>

///////////////////////////////////////////////////////////////////////////////
//  Our program grammar and compiler
///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
struct program : boost::spirit::qi::grammar<Iterator, white_space<Iterator> >
{
    program(std::vector<int>& code);

    typedef white_space<Iterator> white_space_;

    std::vector<int>& code;
    boost::spirit::qi::rule<Iterator, std::string(), white_space_> identifier;
    boost::spirit::qi::rule<Iterator, white_space_> start;

    typedef
    	boost::spirit::qi::locals<
            std::string // function name
          , int         // address
        >
    function_locals;

    boost::spirit::qi::symbols<char, function_info> functions;
    statement<Iterator> statement_;

    boost::spirit::qi::rule<Iterator, function_locals, white_space_>
    	function_definition;
    boost::phoenix::function<function_adder> add_function;
    boost::phoenix::function<function_state_reset> state_reset;
    boost::phoenix::function<compile_op> op;
};


#endif /* PROGRAM_HPP_ */
