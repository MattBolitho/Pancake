// Copyright (c) 2021 Matt Bolitho
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <fstream>
#include <sstream>
#include "pancake.hpp"

constexpr static auto UsageInformation = "Pancake usage:\n\
\n\
pancake <path to input file>\n\
\n\
--version       - Display version number.\n\
--help          - Display this text.";

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << UsageInformation << std::endl;
        return -1;
    }
    auto const argument = std::string(argv[1]);

    if (argument == "--help")
    {
        std::cout << UsageInformation << std::endl;
        return 0;
    }

    if (argument == "--version")
    {
        std::cout << PANCAKE_VERSION << std::endl;
        return 0;
    }

    std::ifstream input(argument);
    if (!input.good())
    {
        std::cerr << "Could not open input file." << std::endl;
        return -1;
    }

    std::stringstream programStream;
    programStream << input.rdbuf();
    auto program = programStream.str();
    input.close();

    auto interpreter = Pancake::PancakeInterpreter();
    interpreter.Interpret(program);

    return 0;
}