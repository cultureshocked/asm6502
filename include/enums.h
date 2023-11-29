#ifndef ASM6502_ENUMS_H
#define ASM6502_ENUMS_H

__attribute__((unused))
typedef enum types {
  NUM, REG, ADDR, LL, NIL,
  IADDR = ADDR | 0x80,
  IREG = REG | 0x80
} ARG_TYPE;

typedef enum registers {
  A = 0x41, X = 0x58, Y = 0x59
} REGISTER;

__attribute__((unused))
typedef enum modes {
  ACC, ABS, ABSX, ABSY,
  IMM, IMPL, IND, INDX, INDY,
  REL, ZPG, ZPGX, ZPGY
} MODE;

typedef enum op {
  ADC = 0x434441, AND = 0x444e41, ASL = 0x4c5341, BCC = 0x434342,
  BCS = 0x534342, BEQ = 0x514542, BIT = 0x544942, BMI = 0x494d42,
  BNE = 0x454e42, BPL = 0x4c5042, BRK = 0x4b5242, BVC = 0x435642,
  BVS = 0x535642, CLC = 0x434c43, CLD = 0x444c43, CLI = 0x494c43,
  CLV = 0x564c43, CMP = 0x504d43, CPX = 0x585043, CPY = 0x595043,
  DEC = 0x434544, DEX = 0x584544, DEY = 0x594544, EOR = 0x524f45,
  INC = 0x434e49, INX = 0x584e49, INY = 0x594e49, JMP = 0x504d4a,
  JSR = 0x52534a, LDA = 0x41444c, LDX = 0x58444c, LDY = 0x59444c,
  LSR = 0x52534c, NOP = 0x504f4e, ORA = 0x41524f, PHA = 0x414850,
  PHP = 0x504850, PLA = 0x414c50, PLP = 0x504c50, ROL = 0x4c4f52,
  ROR = 0x524f52, RTI = 0x495452, RTS = 0x535452, SBC = 0x434253,
  SEC = 0x434553, SED = 0x444553, SEI = 0x494553, STA = 0x415453,
  STX = 0x585453, STY = 0x595453, TAX = 0x584154, TAY = 0x594154,
  TSX = 0x585354, TXA = 0x415854, TXS = 0x535854, TYA = 0x415954
} OPCODE;

struct asm_arg {
  unsigned short int value;
  unsigned short int type;
};

struct asm_op {
  unsigned int opcode;
  int mode;
  struct asm_arg a;
  struct asm_arg b;
};

typedef struct asm_op OP;
typedef struct asm_arg ARGUMENT;

#endif // ASM6502_ENUMS_H
