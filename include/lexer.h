#ifndef ASM6502_LEXER_H
#define ASM6502_LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "enums.h"
#include "labels.h"
#include "helper.h"
#include "program_list.h"

OP parse_line(const char* str, size_t* pos);
ARGUMENT parse_argument(const char* str, size_t* pos);
unsigned short int parse_numeric(const char* str, size_t* pos);
unsigned short int parse_register(const char* str, size_t* pos);
int parse_instruction(const char* str, size_t* pos);
void lex(char* code, LABEL* offset_table, PROGRAM* symbolic_code);

#endif // ASM6502_LEXER_H
