# Pancake

<div style="display: inline;">
<img src="https://img.shields.io/badge/version-0.1.0-blue"/>
<img src="https://img.shields.io/badge/license-MIT-brightgreen"/>
</div>

Pancake is a toy, esoteric programming language.
It is loosely inspired by Brainf*ck, but uses a stack as its basis for computation rather than an array.

```
`Is an input a multiple of 2? Print 'y' or 'n'`
^{2},%z{y}^{110}j{o}:{y}^{121}:{o}.
```

But how did that work?

```
^{2}        `Push 2 to the stack`
,           `Read a single word (64-bits) of input`
%           `Modulo the top of the stack with the second element`
z{y}        `Jump to a label named 'y' if the top of the stack is zero`
^{110}      `Push 110 (ASCII 'n') to the stack`
j{o}        `Unconditional jump to label 'o'`
:{y}        `The label 'y'`
^{121}      `Push 121 (ASCII 'y') to the stack`
:{o}        `The label 'o'`
.           `Print the top word of the stack as an ASCII character`
```

Each instruction can be found documented in the [Pancake language docs](./docs/Language.md).
This contains information on how exactly each instruction affects the stack.

## Getting Started
To get started, build the Pancake interpreter using the CMake project at the root of the repository.
For example:

```sh
mkdir build
cd build
cmake ..
make
```

At this point, it would probably be useful to refer to the [Pancake language docs](./docs/Language.md).
After that, write some Pancake code and run it using the interpreter:

```sh
echo ",,+_" >> example.pnck
pancake example.pnck
```

## Future Work
Pancake is a toy but there are a lot of improvements that could be made:
- Tests
- Better push instructions
- Adding functions
- Logical and relational instructions
- Editor plugins
- Compilation
- Clearer error messages
- Debugging
- Better expose the Pancake virtual machine to allow other languages to target it
- Improve docs
- Write example programs

## License
Pancake is distributed under the MIT license. See [`LICENSE`](./LICENSE) for more information.
