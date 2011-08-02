/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef FUNCTION_ADDER_HPP_
#define FUNCTION_ADDER_HPP_

#include "function_info.hpp"

#include <boost/spirit/include/qi.hpp>

///////////////////////////////////////////////////////////////////////////////
//  A functor that adds functions to our (function) symbol-table
///////////////////////////////////////////////////////////////////////////////
struct function_adder
{
    template <typename, typename, typename>
    struct result { typedef void type; };

    function_adder(boost::spirit::qi::symbols<char, function_info>& functions)
      : functions(functions)
    {
    }

    void operator()(std::string const& function_id, int arity, int address) const
    {
        function_info info = {arity, address};
        functions.add(function_id.begin(), function_id.end(), info);
    };

    boost::spirit::qi::symbols<char, function_info>& functions;
};



#endif /* FUNCTION_ADDER_HPP_ */
