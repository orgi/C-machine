/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_MINI_C)
#define BOOST_SPIRIT_MINI_C

#include "white_space.hpp"

#include <string>

///////////////////////////////////////////////////////////////////////////////
//  Our main compiler
///////////////////////////////////////////////////////////////////////////////
template <typename Grammar>
bool compile(Grammar const& prog, std::string const& expr)
{
    typedef white_space<std::string::const_iterator> white_space;
    white_space ws; //  Our skipper

    std::string::const_iterator iter = expr.begin();
    std::string::const_iterator end = expr.end();
    bool r = phrase_parse(iter, end, prog, ws);
//    bool r = phrase_parse(iter, end, prog, boost::spirit::ascii::space);

    return r && iter == end;
}

int Cmachine(std::string const& fileName);
int execute(std::string source_code);

#endif


