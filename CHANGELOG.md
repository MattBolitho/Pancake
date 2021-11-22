# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## 🔖[0.3.0] - Unreleased
### ✨ Added
- Heap memory I/O - buffers of a fixed size can be allocated
- Memory instructions:
    - Allocate and free heap memory
    - Write values to and dereference values from an index in memory

## 🔖[0.2.0] - 2021-11-16
### ✨ Added
- PANics that can be raised and handled by the user
- Relational and logical instructions
- Pancake docker file

### 🙌 Improvements
- Errors are better formalized as PANics

### 👋 Removed
- Jump if greater/less than instructions (use relational instructions and jump if zero)

## 🔖[0.1.0] - 2021-11-15
### ✨ Added
- First Version 🎂🎉
- Initial Pancake interpreter
    - Arithmetic, bitwise, control flow and I/O instructions
    - Allows for block comments
- Initial Pancake virtual machine
- First specifications of the virtual machine and language