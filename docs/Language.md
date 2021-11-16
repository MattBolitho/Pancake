# Language
This documentation contains information about the Pancake language.

## General Notes
- Pancake source files are text files.
- The standard file extension is `.pnck`.
- Pancake is whitespace agnostic - the whitespace characters (and comments) in input text files will be removed prior to running any code.

## Language Features
Most instructions in Pancake are a single character but there are a few other concepts such as comments, PANics and labels.

### Comments
Comments are wrapped in grave accent characters. For example:

```
`This program reads two inputs and adds them, printing the result.`
,,+_
```

### PANic (Errors)
PANics may come about for some of the following reasons:
- Invalid stack state for instruction. E.g. attempting to add with an empty stack.
- Unrecognised opcode.
- Unmatched label braces.
- Unmatched comment characters.
- Attempting to jump to a label that does not exist in the program.
- User PANic thrown.

### Labels
Some instructions use labels.
Labels allow for instructions to receive arguments.
This allows for specific numbers to be pushed onto the operand stack, named data storage and goto instructions.

Labels are given in the form `{LABEL}`. For example:

```
`This is an infinite loop. It declares a label called "MyLabel" and jumps to it uncoditionally forever.`
:{MyLabel}
j{MyLabel}
```

Numbers are the expected input for the push instruction:

```
`Use numbers in labels to push specific values to the stack.`
^{48}_
```

However, if no number is provided, then `0` will be pushed.

```
^
`The stack top is now 0`
```

### Instruction Table
The following tables contain information about each instruction in Pancake.

Note that where an instruction requires a label, this is simply given as `{LABEL}`.
Where stack transitions are given, they are given as left to right equals top to bottom.
Values in stack transition cells are space delimited.

For example `a b c` represents the following stack:
```
a   <- Top
b
c
```

#### Stack Manipulation

| Name | Instruction | Notes | Stack Transition |
| ---- | ----------- | ----- | ---------------- |
| Push | `^{VALUE}` | Pushes a value to the stack. Value can be omitted and defaults to `0`. | `a -> VALUE a` |
| Pop | `;` | Removes the top value of the stack. | `a b -> b` |
| Duplicate | `&` | Duplicates the top value of the stack. | `a -> a a` |
| Swap | `$` | Swaps the top two values of the stack. | `a b -> b a` |
| Reverse | `~` | Reverses the values on the stack. | `a b c -> c b a` |
| Over | `'` | Pushes a copy of the second value of the stack to the top of the stack. | `a b -> b a b` |

#### Arithmetic Operations

| Name | Instruction | Notes | Stack Transition |
| ---- | ----------- | ----- | ---------------- |
| Add | `+` | Adds the top two values of the stack. | `a b -> (a + b)` |
| Subtract | `-` | Subtracts the top two values of the stack. | `a b -> (a - b)` |
| Multiplication | `*` | Multiplies the top two values of the stack. | `a b -> (a * b)` |
| Division | `/` | Divides the top two values of the stack. | `a b -> (a / b)` |
| Modulo | `%` | Calculates the division remainder of the top two values of the stack. | `a b -> (a % b)` |
| Increment | `>` | Increments the top value of the stack. | `a -> (a + 1)` |
| Decrement | `<` | Decrements the top value of the stack. | `a -> (a - 1)` |

#### Bitwise Operations

| Name | Instruction | Notes | Stack Transition |
| ---- | ----------- | ----- | ---------------- |
| Left Shift | `[` | Left shifts the top of the stack by the second value of the stack. | `a b -> (a << b)` |
| Right Shift | `]` | Right shifts the top of the stack by the second value of the stack. | `a b -> (a >> b)` |
| Bitwise Negation | `n` | Bitwise negation (one's complement) of the top value of the stack. | `a -> (~a)` |
| Bitwise And | `a` | Bitwise and of the top two values of the stack. | `a b -> (a & b)` |
| Bitwise Inclusive Or | `o` | Bitwise inclusive or of the top two values of the stack. | `a b -> (a \| b)` |
| Bitwise Exclusive Or | `x` | Bitwise exclusive or (XOR) of the top two values of the stack. | `a b -> (a ^ b)` |

#### Logical Operations
**Note**: Logical operations will convert the value of words to "boolean" like values.
The conversion is the same as C, where `0 == false` and any non-zero value is true.

| Name | Instruction | Notes | Stack Transition |
| ---- | ----------- | ----- | ---------------- |
| Equals | `E` | Pushes a value indicating if the top two values of the stack are equal. | `a b -> A && B` |
| Greater Than | `G` | Pushes a value indicating if the top value of the stack is strictly greater than the second. | `a b -> A > B` |
| Less Than | `L` | Pushes a value indicating if the top value of the stack is strictly less than the second. |`a b -> A < B` |
| Greater Than Or Equal | `g` | Pushes a value indicating if the top value of the stack is greater than or equal to the second. | `a b -> A >= B` |
| Less Than Or Equal | `l` | Pushes a value indicating if the top value of the stack is less than or equal to the second. | `a b -> A <= B` |
| Not | `N` | Logically negates the top value of the stack. | `a -> !a` |
| And | `A` | Performs logical conjunction of the top two values of the stack. | `a b -> (a && b)` |
| Inclusive Or | `O` | Performs logical disjunction of the top two values of the stack. | `a b -> (a \|\| b)` |
| Exclusive Or | `X` | Performs logical exclusive disjunction of the top two values of the stack. | `a b -> (!a != !b)` |

#### Flow of Control Operations
**Note**: Jump instructions do not affect the stack.

| Name | Instruction | Notes | Stack Transition |
| ---- | ----------- | ----- | ---------------- |
| Terminate | `\|` | Terminates the program. | N/A |
| PANic | `p{LABEL}` | User defined PANic error named `LABEL`. | N/A |
| Handle | `h{LABEL}` | Handles a user defined PANic named `LABEL`. | N/A |
| Label | `:{LABEL}` | Marks a label as a jump address. | N/A |
| Unconditional Jump | `j{LABEL}` | Unconditional jump to the `LABEL`. | N/A |
| Jump If Zero | `z{LABEL}` | Jumps to the label `LABEL` if the value at the top of the stack is `0`. | N/A |
| Jump If Equal | `e{LABEL}` | Jumps to the label `LABEL` if the two top values of the stack are equal. | N/A |

#### I/O Operations

| Name | Instruction | Notes | Stack Transition |
| ---- | ----------- | ----- | ---------------- |
| Output ASCII | `.` | Writes the top of the stack to `stdout` as a character. | `a -> ` |
| Output Literal | `_` | Writes the top of the stack to `stdout` as a literal value. | `a -> ` |
| Input Word | `,` | Accepts a single machine word as input from `stdin` and pushes it to the operand stack. | `a -> INPUT a` |
| Store | `!{LABEL}` | Pops and stores the top of the stack into memory under the name `LABEL`. | `a -> ` |
| Load | `?{LABEL}` | Pushes the value stored under the name `LABEL` to the operand stack. | `a -> LOADED_VALUE a` |
