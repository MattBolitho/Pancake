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

#ifndef PANCAKE_HPP
#define PANCAKE_HPP

#define PANCAKE
#define PANCAKE_VERSION "0.1.0"

#include <cstdint>
#include <cstdio>
#include <string>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <functional>

namespace Pancake
{
    /// The size of a single word in the stack.
    using Word = uint64_t;

    /// The type used for the operand stack.
    using Stack = std::stack<Word>;

    /// Memory is a store of named values.
    using Memory = std::unordered_map<std::string, Word>;

    /// Stores jump addresses for labels.
    using Labels = std::unordered_map<std::string, std::string::size_type>;

    /// Thrown when an error occurs evaluating a Pancake program.
    class PancakeRuntimeException final : public std::runtime_error
    {
        public:
            /// Initializes a new instance of the PancakeRuntimeException class.
            /// @param message The message.
            explicit PancakeRuntimeException(char const* message)
                : std::runtime_error(message)
            {
            }

            /// Initializes a new instance of the PancakeRuntimeException class.
            /// @param message The message.
            explicit PancakeRuntimeException(std::string const& message)
                : std::runtime_error(message)
            {
            }
    };

    /// A stack virtual machine architecture for the Pancake programming language.
    class PancakeVirtualMachine final
    {
        public:
            /// Gets a value indicating whether or not a program is running on the virtual machine.
            bool IsRunning() const noexcept
            {
                return _running;
            }

            /// Gets the current value of the instruction pointer.
            std::string::size_type GetInstructionPointer() const noexcept
            {
                return _instructionPointer;
            }

            /// Initializes the virtual machine ready for instructions to be dispatched.
            void InitializeForNewProgram(std::string const& program)
            {
                _running = true;
                _instructionPointer = 0;
                _program = program;
                _stack = Stack();
                _memory = Memory();
                _labels = Labels();
            }

            /// Dispatches the opocde with no arguments to the virtual machine.
            /// @param opcode The opcode to dispatch.
            void DispatchInstruction(char const opcode)
            {
                switch (opcode)
                {
                    case '|':
                        _running = false;
                        break;

                    case '^':
                        _stack.push(0);
                        break;

                    case ';':
                        VerifyUnaryOperation();
                        _stack.pop();
                        break;

                    case '&':
                        VerifyUnaryOperation();
                        _stack.push(_stack.top());
                        break;

                    case '$':
                    {
                        VerifyBinaryOperation();
                        auto const top = _stack.top();
                        _stack.pop();
                        auto const second = _stack.top();
                        _stack.pop();
                        _stack.push(top);
                        _stack.push(second);
                        break;
                    }

                    case '~':
                        ReverseStack();
                        break;

                    case '\'':
                    {
                        VerifyBinaryOperation();
                        auto const top = _stack.top();
                        _stack.pop();
                        auto const second = _stack.top();
                        _stack.push(top);
                        _stack.push(second);
                        break;
                    }

                    case '+':
                    {
                        PerformBinaryOperation([](auto a, auto b) { return a + b; });
                        break;
                    }

                    case '-':
                    {
                        PerformBinaryOperation([](auto a, auto b) { return a - b; });
                        break;
                    }

                    case '*':
                    {
                        PerformBinaryOperation([](auto a, auto b) { return a * b; });
                        break;
                    }

                    case '/':
                    {
                        PerformBinaryOperation([](auto a, auto b) { return a / b; });
                        break;
                    }

                    case '%':
                    {
                        PerformBinaryOperation([](auto a, auto b) { return a % b; });
                        break;
                    }

                    case '>':
                        VerifyUnaryOperation();
                        ++_stack.top();
                        break;

                    case '<':
                        VerifyUnaryOperation();
                        --_stack.top();
                        break;

                    case '[':
                        VerifyBinaryOperation();
                        PerformBinaryOperation([](auto a, auto b) { return a << b; });
                        break;

                    case ']':
                        VerifyBinaryOperation();
                        PerformBinaryOperation([](auto a, auto b) { return a >> b; });
                        break;

                    case 'n':
                    {
                        VerifyUnaryOperation();
                        auto const top = _stack.top();
                        _stack.pop();
                        _stack.push(~top);
                        break;
                    }

                    case 'a':
                        VerifyBinaryOperation();
                        PerformBinaryOperation([](auto a, auto b) { return a & b; });
                        break;

                    case 'o':
                        VerifyBinaryOperation();
                        PerformBinaryOperation([](auto a, auto b) { return a | b; });
                        break;

                    case 'x':
                        VerifyBinaryOperation();
                        PerformBinaryOperation([](auto a, auto b) { return a ^ b; });
                        break;

                    case '.':
                        VerifyUnaryOperation();
                        std::cout << static_cast<char>(_stack.top());
                        _stack.pop();
                        break;

                    case '_':
                        VerifyUnaryOperation();
                        std::cout << std::to_string(_stack.top());
                        _stack.pop();
                        break;

                    case ',':
                    {
                        std::string value;
                        std::cin >> value;
                        _stack.push(static_cast<Word>(std::stoull(value)));
                        break;
                    }

                    default:
                        ThrowForUnrecognisedOpcode(opcode);
                        break;
                }

                ++_instructionPointer;
            }

            /// Dispatches the opocde with a byte argument to the virtual machine.
            /// @param opcode The opcode to dispatch.
            /// @param value The byte value in the instruction.
            void DispatchWordInstruction(char const opcode, Word const value)
            {
                if (opcode == '^')
                {
                    _stack.push(value);
                    _instructionPointer += std::to_string(value).size() + 3; // todo - improve this
                }
                else
                {
                    ThrowForUnrecognisedOpcode(opcode);
                }
            }

            /// Dispatches the opocde with a label argument to the virtual machine.
            /// @param opcode The opcode to dispatch.
            /// @param label The label in the instruction.
            void DispatchLabelInstruction(char const opcode, std::string const& label)
            {
                switch (opcode)
                {
                    case ':':
                    {
                        auto const jumpAddress = _instructionPointer + label.size() + 3;
                        _labels[label] = jumpAddress;
                        _instructionPointer = jumpAddress;
                        break;
                    }

                    case 'j':
                        Jump(label);
                        break;

                    case 'z':
                        VerifyUnaryOperation();
                        ConditionalJump(_stack.top() == 0, label);
                        break;

                    case 'e':
                    {
                        VerifyBinaryOperation();
                        auto const top = _stack.top();
                        _stack.pop();
                        ConditionalJump(top == _stack.top(), label);
                        _stack.push(top);
                        break;
                    }

                    case 'g':
                    {
                        VerifyBinaryOperation();
                        auto const top = _stack.top();
                        _stack.pop();
                        ConditionalJump(top > _stack.top(), label);
                        _stack.push(top);
                        break;
                    }

                    case 'l':
                    {
                        VerifyBinaryOperation();
                        auto const top = _stack.top();
                        _stack.pop();
                        ConditionalJump(top < _stack.top(), label);
                        _stack.push(top);
                        break;
                    }

                    case '!':
                        VerifyUnaryOperation();
                        _memory[label] = _stack.top();
                        _stack.pop();
                        _instructionPointer += label.size() + 3;
                        break;

                    case '?':
                        VerifyRead(label);
                        _stack.push(_memory[label]);
                        _instructionPointer += label.size() + 3;
                        break;

                    default:
                        ThrowForUnrecognisedOpcode(opcode);
                        break;
                }
            }

        private:
            bool _running = false;
            std::string::size_type _instructionPointer = 0;
            std::string _program;
            Stack _stack{};
            Memory _memory{};
            Labels _labels{};

            void VerifyUnaryOperation() const
            {
                if (_stack.empty())
                {
                    throw PancakeRuntimeException("Attempted to perform unary operation on empty stack.");
                }
            }

            void VerifyBinaryOperation() const
            {
                if (_stack.size() < 2)
                {
                    throw PancakeRuntimeException("Attempted to perform binary operation with fewer than 2 values on the stack.");
                }
            }

            void VerifyRead(std::string const& label) const
            {
                if (_memory.find(label) == _memory.end())
                {
                    std::stringstream errorStream;
                    errorStream << "No value stored with name '" << label << "' could be found in memory.\n";
                    throw PancakeRuntimeException(errorStream.str());
                }
            }

            void PerformBinaryOperation(std::function<Word(Word, Word)> const& operation)
            {
                // Note: the function operands are given in the order (top, second).

                VerifyBinaryOperation();
                auto const a = _stack.top();
                _stack.pop();
                auto const b = _stack.top();
                _stack.pop();
                _stack.push(operation(a, b));
            }

            void ReverseStack()
            {
                std::stack<Word> newStack{};
                while (!_stack.empty())
                {
                    newStack.push(_stack.top());
                    _stack.pop();
                }
                _stack = newStack;
            }

            void Jump(std::string const& label)
            {
                if (_labels.find(label) != _labels.end())
                {
                    _instructionPointer = _labels[label];
                    return;
                }

                std::stringstream expectedLabelStringStream;
                expectedLabelStringStream << ":{" << label << "}";
                auto labelInstruction = expectedLabelStringStream.str();
                auto foundIndex = _program.find(labelInstruction);
                if (foundIndex == std::string::npos)
                {
                    throw PancakeRuntimeException(std::string("Cannot jump to label '") + label + "' as it does not exist.");
                }

                auto jumpIndex = foundIndex + labelInstruction.size();
                _labels[label] = jumpIndex;
                _instructionPointer = jumpIndex;
            }

            void ConditionalJump(bool const condition, std::string const& label)
            {
                // If the condition is true, we need to find the label and jump there.
                // Otherwise we need to do the usual instruction pointer arithmetic
                // over the current instruction, label and curly brace characters.

                if (condition)
                {
                    Jump(label);
                }
                else
                {
                    _instructionPointer += label.size() + 3;
                }
            }

            static void ThrowForUnrecognisedOpcode(char const opcode)
            {
                std::stringstream errorStream;
                errorStream << "Unrecognised opcode - '" << opcode << "'.\n";
                throw PancakeRuntimeException(errorStream.str());
            }
    };

    /// Interprets Pancake programs.
    class PancakeInterpreter final
    {
        public:
            /// Runs the instructions in the given program until they
            /// are exhausted or an error is encountered.
            /// @param program The string containing the program.
            void Interpret(std::string& program)
            {
                try
                {
                    PreProcessProgram(program);
                    _virtualMachine.InitializeForNewProgram(program);

                    while (_virtualMachine.IsRunning())
                    {
                        auto const instructionPointer = _virtualMachine.GetInstructionPointer();
                        auto const instruction = program[instructionPointer];
                        if (instruction == '\0')
                        {
                            break;
                        }

                        if (program[instructionPointer + 1] == '{')
                        {
                            auto nextClosingBraceIndex = program.find('}', instructionPointer);
                            if (nextClosingBraceIndex == std::string::npos)
                            {
                                throw PancakeRuntimeException("Unmatched braces for argument instruction.");
                            }
                            auto argumentStartIndex = instructionPointer + 2;
                            auto argument = program.substr(argumentStartIndex, nextClosingBraceIndex - argumentStartIndex);

                            if (instruction == '^')
                            {
                                auto value = static_cast<Pancake::Word>(std::stoi(argument));
                                _virtualMachine.DispatchWordInstruction(instruction, value);
                            }
                            else
                            {
                                _virtualMachine.DispatchLabelInstruction(instruction, argument);
                            }
                        }
                        else
                        {
                            _virtualMachine.DispatchInstruction(instruction);
                        }
                    }
                }
                catch (PancakeRuntimeException const& pancakeException)
                {
                    std::cerr << "Pancake runtime error: " << pancakeException.what() << std::endl;
                }
                catch (...)
                {
                    std::cerr << "Unspecified error." << std::endl;
                }
            }

        private:
            PancakeVirtualMachine _virtualMachine{};

            static void PreProcessProgram(std::string& program)
            {
                /// Remove standard whitespace characters.
                std::unordered_set<char> charactersToRemove = { '\n', '\r', ' ', '\t' };
                auto removeWhitespaceFunction = [&](char const c) { return charactersToRemove.find(c) != charactersToRemove.end(); };
                auto endPosition = std::remove_if(program.begin(), program. end(), removeWhitespaceFunction);
                program.erase(endPosition, program.end());

                // Remove comments.
                while (true)
                {
                    auto const commentStart = program.find('`');
                    if (commentStart == std::string::npos)
                    {
                        break;
                    }

                    auto const commentEnd = program.find('`', commentStart + 1);
                    if (commentEnd == std::string::npos)
                    {
                        throw PancakeRuntimeException("Unmatched comment.");
                    }

                    program.erase(commentStart, commentEnd - commentStart + 1);
                }
            }
    };
}

#endif