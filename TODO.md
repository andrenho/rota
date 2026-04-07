# Design

## Language (Rota)

- Minimal
- Supported types: int (signed), float, byte (unsigned), string, table, array, function
- Internals inspired by Lua
- Syntax inspired by Go
- Eventually a superlanguage will be created 

## VM

Stack based.

VM memory organization:
  - Code
  - Stack
  - Heap (general objects) - GC support
  - Strings (all strings in code)

- Interpreter: run code directly, mostly used for testing the compiler
- Compiler -> assembler -> bytecode: normal cycle

Things included in the VM:

- The VM itself
- Support for slow/complex activities, via opcodes:
  - String manipulation
  - Math operations
  - FAT
  - Hardware operations (serial, video, keyboard, sdcard, wi-fi interfaces)
- Simple DOS-like OS
  - Include a simple text editor (?)
- Compiler
- Debugger

## Hardware

- Version A - connects to a PC
  - Pico Pi 2 W
    - 1 core running VM
  - SDCard
  - Serial interface (power via serial port)
- Version B - desktop
  - Same as version A, plus
    - VGA interface
    - USB interface (for keyboard, maybe mouse)
    - Power management
    - RTC (maybe)
- Version C - laptop (maybe)
  - Same as version A, plus
    - Screen
    - Mini-keyboard
    - Battery

# Phase 1 - interpreter

- [x] Base VM (C++)
  - [x] Interface similar to Lua C API
- [x] Base compiler as interpreter
  - [x] Test code
- [x] T_INT: expressions
- [x] T_COUNT: expressions
  - [x] Modulo
  - [x] Power
  - [x] Integer division
- [ ] Equality, logical
  - [ ] Equality
  - [ ] Logical
    - [ ] Not
- [ ] Local variables
  - [ ] Multiple memories
- [ ] Control flow
- [ ] Functions
- [ ] Strings
  - [ ] Fixed strings
- [ ] Bitwise operators (maybe byte type?)
- [ ] Tables + GC
- [ ] Globals
- [ ] Error handling
- [ ] Closures

# Phase 2 - serial prototype

# Phase 3 - VGA prototype