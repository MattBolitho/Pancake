# Virtual Machine
This documentation contains information about the default Pancake virtual machine.

The virtual machine contains the following components:
- A stack of 64 bit unsigned integers.
- An unsigned 64-bit integer instruction pointer.
- A map of label names to unsigned 64-bit integer jump addresses.
- A map of names to unsigned 64-bit words stored as memory.
- A running flag.
