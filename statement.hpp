/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef STATEMENT_HPP_
#define STATEMENT_HPP_

#include "white_space.hpp"
#include "var_adder.hpp"
#include "expression.hpp"
#include "function_info.hpp"
#include "compile_op.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>

#include <vector>

///////////////////////////////////////////////////////////////////////////////
//  Our statement grammar and compiler
///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
struct statement : boost::spirit::qi::grammar<Iterator, white_space<Iterator> >
{
    statement(std::vector<int>& code,
    		boost::spirit::qi::symbols<char, function_info>& functions);

    typedef white_space<Iterator> white_space_;

    std::vector<int>& code;
    boost::spirit::qi::symbols<char, int> vars;
    boost::spirit::qi::symbols<char, symbol_info> variables;
    boost::spirit::qi::symbols<char, function_info>& functions;
    int nvars;
    bool has_return;

    expression<Iterator> expr;
    boost::spirit::qi::rule<Iterator, white_space_>
        statement_, statement_list, compound_statement
      , return_statement;

    boost::spirit::qi::rule<Iterator, boost::spirit::qi::locals<bool, int>, white_space_> var_decl;
    boost::spirit::qi::rule<Iterator, boost::spirit::qi::locals<int>, white_space_> if_statement;
    boost::spirit::qi::rule<Iterator, boost::spirit::qi::locals<int, int>, white_space_> while_statement;
    boost::spirit::qi::rule<Iterator, int(), boost::spirit::qi::locals<int>, white_space_> print_statement;
    boost::spirit::qi::rule<Iterator, std::string(), white_space_> identifier;
    boost::spirit::qi::rule<Iterator, int(), white_space_> var_ref;
    boost::spirit::qi::rule<Iterator, boost::spirit::qi::locals<int>, white_space_> assignment;
    boost::spirit::qi::rule<Iterator, void(int), white_space_> assignment_rhs;

    boost::phoenix::function<var_adder> add_var;
    boost::phoenix::function<var_adder2> add_var_2;
    boost::phoenix::function<compile_op> op;
};

#endif /* STATEMENT_HPP_ */
