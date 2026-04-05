#ifndef FORTUNA_6_OPCODES_HH
#define FORTUNA_6_OPCODES_HH

#define OP_NOP  0x00

// stack operations
#define OP_PUSH 0x01

// arithmetic
#define OP_ADD  0x10
#define OP_SUB  0x11
#define OP_MUL  0x12
#define OP_DIV  0x13

// special operations
#define OP_HALT 0xff

#endif //FORTUNA_6_OPCODES_HH
