/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
///////////////////////////////////////////////////////////////////////////////
//
//  Not a calculator anymore, right? :-)
//
//  [ JDG April 10, 2007 ]       spirit2
//
///////////////////////////////////////////////////////////////////////////////
#include "exec.hpp"
#include "vmachine.hpp"
#include "program.hpp"

#include <fstream>

int execute(std::string source_code)
{
    typedef std::string::const_iterator iterator_type;
    typedef program<iterator_type> program;

    vmachine mach;              //  Our virtual machine
    std::vector<int> code;      //  Our VM code
    program prog(code);         //  Our grammar definition

    int result = 1;

    if (::compile(prog, source_code))
    {
        std::string fmain("main");
        std::string::iterator fbegin = fmain.begin();
        function_info* f = prog.functions.lookup->find(fbegin, fmain.end());
        if (f == 0)
        {
            std::cerr << "Error: main function not defined" << std::endl;
            return 1;
        }

        result = mach.execute(
            code                                // code
          , code.begin() + f->address           // pc
          , mach.stack.begin()                  // frame_ptr
        );

    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////
//  Main program
///////////////////////////////////////////////////////////////////////////////
int Cmachine(std::string const& fileName)
{
    std::ifstream in(fileName.c_str(), std::ios_base::in);

    if (!in)
    {
        std::cerr << "Error: Could not open input file: "
            << fileName << std::endl;
        return 1;
    }

    std::string source_code; // We will read the contents here.
    in.unsetf(std::ios::skipws); // No white space skipping!
    std::copy(
        std::istream_iterator<char>(in),
        std::istream_iterator<char>(),
        std::back_inserter(source_code));

    return execute(source_code);
}


