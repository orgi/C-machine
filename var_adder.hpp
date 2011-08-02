/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/


#ifndef VAR_ADDER_HPP_
#define VAR_ADDER_HPP_

#include <boost/spirit/include/qi.hpp>

struct symbol_info
{
    int id;
    int size;
    bool sign;
};

///////////////////////////////////////////////////////////////////////////////
//  A functor that adds variables to our (variables) symbol-table
///////////////////////////////////////////////////////////////////////////////
struct var_adder
{
    template <typename>
    struct result { typedef void type; };

    var_adder(boost::spirit::qi::symbols<char, int>& vars, int& nvars)
      : vars(vars), nvars(nvars)
    {
    }

    void operator()(std::string const& var) const
    {
        vars.add(var.begin(), var.end(), nvars++);
    };

    boost::spirit::qi::symbols<char, int>& vars;
    int& nvars;
};

///////////////////////////////////////////////////////////////////////////////
//  A functor that adds variables to our (variables) symbol-table including a data type!
///////////////////////////////////////////////////////////////////////////////
struct var_adder2
{
    template <typename, typename, typename>
    struct result { typedef void type; };

    var_adder2(boost::spirit::qi::symbols<char, int>& oldVars,
    		boost::spirit::qi::symbols<char, symbol_info>& vars, int& nvars)
      : oldVars(oldVars), vars(vars), nvars(nvars)
    {
    }

    void operator()(std::string const& var, const int size, const bool sign) const
    {
      oldVars.add(var.begin(), var.end(), nvars);
      symbol_info info = {nvars++, size, sign};
      vars.add(var.begin(), var.end(), info);
    };

    boost::spirit::qi::symbols<char, int>& oldVars;
    boost::spirit::qi::symbols<char, symbol_info>& vars;
    int& nvars;
};


#endif /* VAR_ADDER_HPP_ */
