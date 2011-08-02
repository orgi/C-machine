/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef WHITE_SPACE_HPP_
#define WHITE_SPACE_HPP_

#include <boost/spirit/include/qi.hpp>

///////////////////////////////////////////////////////////////////////////////
//  White-space and comments grammar definition
///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
struct white_space : boost::spirit::qi::grammar<Iterator>
{
    white_space() : white_space::base_type(start)
    {
        using boost::spirit::ascii::char_;

        start =
                boost::spirit::qi::space            // tab/space/cr/lf
            |   "/*" >> *(char_ - "*/") >> "*/"     // C-style comments
            ;

          // Enable debugging
//        debug(start);
    }

    boost::spirit::qi::rule<Iterator> start;
};

#endif /* WHITE_SPACE_HPP_ */
