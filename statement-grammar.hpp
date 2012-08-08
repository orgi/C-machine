/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(STATEMENT_GRAMMAR_HPP)
#define STATEMENT_GRAMMAR_HPP

#include "statement.hpp"
#include "error_handler.hpp"

#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_container.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>

///////////////////////////////////////////////////////////////////////////////
//  Our statement grammar and compiler
///////////////////////////////////////////////////////////////////////////////


namespace{
  struct InternalFunctions
    :public boost::spirit::qi::symbols<char, int>
  {
    InternalFunctions()
    {
      add("print", op_print);
      add("dump", op_dump);
      add("debug", op_debug);
    }
  } internalFunctions;
}

template <typename Iterator>
statement<Iterator>::statement(
    std::vector<int>& code,
    	boost::spirit::qi::symbols<char, function_info>& functions)
  : statement::base_type(statement_list)
  , code(code)
  , functions(functions)
  , nvars(0)
  , expr(code, vars, functions)
  , add_var(var_adder(vars, nvars))
  , add_var_2(var_adder2(vars, variables, nvars))
  , op(code)
{
  using boost::phoenix::ref;
  using namespace boost::spirit::qi;
  using namespace boost::phoenix;

    identifier %=
        raw[lexeme[alpha >> *(alnum | '_')]]
        ;

    var_ref =
        lexeme
        [
                vars        [_val = _1]
            >>  !(alnum | '_') // make sure we have whole words
        ]
        ;

    var_decl =
        lexeme
        [
            (boost::spirit::ascii::string("int") [_a = true]
              | boost::spirit::ascii::string("uint") [_a = false]
                )
            >> boost::spirit::ushort_ [_b = _1 / 8]
            >> "_t"
            >>  !(alnum | '_')  // make sure we have whole words
        ]
        >   !var_ref        // make sure the variable isn't redeclared
        >   identifier      [add_var_2(_1, _b, _a)]
        >   (';' | ('=' > assignment_rhs(ref(nvars)-1)))
        ;

    assignment =
            var_ref         [_a = _1]
        >>  '='
        >   assignment_rhs(_a)
        ;

    assignment_rhs =
            expr
        >   lit(';')        [op(op_store_32, _r1)]
        ;

    if_statement =
            lit("if")
        >>  '('
        >   expr            [
                                op(op_jump_if, 0), // we shall fill this (0) in later
                                _a = boost::phoenix::size(boost::phoenix::ref(code))-1 // mark its position
                            ]
        >   ')'
        >   statement_      [
                                // now we know where to jump to (after the if branch)
                                boost::phoenix::ref(code)[_a] = size(boost::phoenix::ref(code))
                            ]
        >>
           -(
                lexeme[
                    "else"
                    >> !(alnum | '_') // make sure we have whole words
                ]                                [
                                boost::phoenix::ref(code)[_a] += 2, // adjust for the "else" jump
                                op(op_jump, 0), // we shall fill this (0) in later
                                _a = size(boost::phoenix::ref(code))-1 // mark its position
                            ]
            >   statement_  [
                                // now we know where to jump to (after the else branch)
                                boost::phoenix::ref(code)[_a] = size(boost::phoenix::ref(code))
                            ]
            )
        ;

    while_statement =
            lit("while")    [
                                _a = size(boost::phoenix::ref(code)) // mark our position
                            ]
        >>  '('
        >   expr            [
                                op(op_jump_if, 0), // we shall fill this (0) in later
                                _b = size(boost::phoenix::ref(code))-1 // mark its position
                            ]
        >   ')'
        >   statement_      [
                                op(op_jump, _a), // loop back
                                // now we know where to jump to (to exit the loop)
                                boost::phoenix::ref(code)[_b] = size(boost::phoenix::ref(code))
                            ]
        ;

    print_statement =
            internalFunctions[_a = _1]
            //lit("print")
        >>  lit('(')
        >   expr [op(_a)]
        >   lit(')')
        >   ';'
        ;

    compound_statement =
        '{' >> -statement_list >> '}'
        ;

    return_statement =
            lexeme[
                "return"
                >> !(alnum | '_') // make sure we have whole words
            ]
        >> -(
                eps(ref(has_return)) > expr     [op(op_return)]
            )
        >   ';'
        ;


    statement_ =
            var_decl
        |   print_statement
        |   assignment
        |   compound_statement
        |   if_statement
        |   while_statement
        |   return_statement
        ;

    statement_list = +statement_;

    identifier.name("identifier");
    var_ref.name("variable-reference");
    var_decl.name("variable-declaration");
    assignment.name("assignment");
    assignment_rhs.name("assignment-rhs");
    if_statement.name("if-statement");
    while_statement.name("while-statement");
    print_statement.name("print-statement");
    compound_statement.name("compound-statement");
    return_statement.name("return-statement");
    statement_.name("statement");
    statement_list.name("statement-list");

    // enable debugging
//    boost::spirit::qi::debug(statement_list);
//    boost::spirit::qi::debug(statement_);
//    boost::spirit::qi::debug(if_statement);
//    boost::spirit::qi::debug(while_statement);
//    boost::spirit::qi::debug(compound_statement);
//    boost::spirit::qi::debug(return_statement);
//    boost::spirit::qi::debug(var_decl);
//    boost::spirit::qi::debug(var_ref);
//    boost::spirit::qi::debug(assignment);
//    boost::spirit::qi::debug(assignment_rhs);
//    boost::spirit::qi::debug(identifier);

    on_error<fail>(statement_list, ::error_handler(_4, _3, _2));
}

#endif // STATEMENT_GRAMMAR_HPP


