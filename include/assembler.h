#ifndef ASM6502_ASSEMBLER_H
#define ASM6502_ASSEMBLER_H

#include "enums.h"
#include "instruction_handlers.h"
#include "helper.h"
#include "program_list.h"

void assemble_program(PROGRAM* p, const char* filename);

#endif // ASM6502_ASSEMBLER_H