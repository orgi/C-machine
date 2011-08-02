/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef EXPRESSION_HPP_
#define EXPRESSION_HPP_

#include "white_space.hpp"
#include "function_info.hpp"
#include "compile_op.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>

#include <vector>

///////////////////////////////////////////////////////////////////////////////
//  Our expression grammar and compiler
///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
struct expression : boost::spirit::qi::grammar<Iterator, white_space<Iterator> >
{
    expression(
        std::vector<int>& code
      , boost::spirit::qi::symbols<char, int>& vars
      , boost::spirit::qi::symbols<char, function_info>& functions);

    typedef white_space<Iterator> white_space_;

    boost::spirit::qi::rule<Iterator, white_space_>
        expr, equality_expr, relational_expr
      , logical_expr, additive_expr, multiplicative_expr
      , unary_expr, primary_expr, variable, print
    ;

    boost::spirit::qi::rule<Iterator,
    	boost::spirit::qi::locals<function_info, int>, white_space_> function_call;

    std::vector<int>& code;
    boost::spirit::qi::symbols<char, int>& vars;
    boost::spirit::qi::symbols<char, function_info>& functions;
    boost::phoenix::function<compile_op> op;
};

#endif /* EXPRESSION_HPP_ */
