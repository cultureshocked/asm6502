#include <stdio.h>
#include <stdlib.h>

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

int cmp(const void*, const void*);

int main(void) {

  int arr[56] = {
      ADC, AND, ASL, BCC, BCS, BEQ, BIT, BMI,
      BNE, BPL, BRK, BVC, BVS, CLC, CLD, CLI,
      CLV, CMP, CPX, CPY, DEC, DEX, DEY, EOR,
      INC, INX, INY, JMP, JSR, LDA, LDX, LDY,
      LSR, NOP, ORA, PHA, PHP, PLA, PLP, ROL,
      ROR, RTI, RTS, SBC, SEC, SED, SEI, STA,
      STX, STY, TAX, TAY, TSX, TXA, TXS, TYA
  };

  qsort(arr, 56, sizeof(int), cmp);

  putchar('{');
  for (int i = 0; i < 56; ++i) {
    printf("%s, ", (char*)(arr+i));
  }
  putchar('}');

}

int cmp(const void* a, const void* b) {
  return (* (int*) a) - (* (int*) b);
}