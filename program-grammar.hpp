/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(PROGRAM_GRAMMAR_HPP)
#define PROGRAM_GRAMMAR_HPP

#include "program.hpp"
#include "error_handler.hpp"

#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_container.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>

///////////////////////////////////////////////////////////////////////////////
//  Our program grammar and compiler
///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
program<Iterator>::program(std::vector<int>& code)
  : program::base_type(start)
  , code(code)
  , statement_(code, functions)
  , add_function(function_adder(functions))
  , state_reset(function_state_reset(code, statement_.vars, statement_.nvars))
  , op(code)
{
	using namespace boost::spirit::qi;
	//using namespace boost::spirit;
	//using namespace boost::phoenix;

	//using boost::phoenix::function;
	using boost::phoenix::ref;
	using boost::phoenix::size;

    bool& has_return = statement_.has_return;
    int& nvars = statement_.nvars;
    boost::phoenix::function<var_adder>& add_var = statement_.add_var;

    identifier %=
        raw[lexeme[alpha >> *(alnum | '_')]]
        ;

        using boost::phoenix::ref;
    function_definition =
            (
                lit("void")                     [ref(has_return) = false]
            |   lit("int")                      [ref(has_return) = true]
            )
        >>  !functions                          // no duplicate functions!
        >>  identifier                          [_a = _1]
        >>  '('
        >  -(
                identifier                      [add_var(_1)]
                >> *(',' > identifier           [add_var(_1)])
            )
        >   ')'
        >   lit('{')                            [ // actually this one requires the phoenix
                                                  // statement include which overrides
                                                  // the comma operator :-/
                                                    _b = size(boost::phoenix::ref(code)),
                                                    add_function(
                                                        _a     // function name
                                                      , ref(nvars)      // arity
                                                      , size(boost::phoenix::ref(code)) // address
                                                    ),
                                                    op(op_stk_adj, 0)   // adjust this later
                                                ]
        >   statement_
        >   lit('}')                            [state_reset(_b)]
        ;

    start =
        +function_definition
        ;

    identifier.name("identifier");
    function_definition.name("function_definition");
    start.name("program");

    on_error<fail>(start, ::error_handler(_4, _3, _2));
}

#endif // PROGRAM_GRAMMAR_HPP


