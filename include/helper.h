#ifndef ASM6502_HELPER_H
#define ASM6502_HELPER_H

#include <stddef.h>
#include "enums.h"

unsigned short int atoh(const char* str, int len);
unsigned short int antoi(const char* str, int len);
unsigned short int atoreg(const char ch);
unsigned short int atob(const char* str, int len);

int rstrip(char* str, size_t len);
int count_lines(const char* str);
void upcase(char* str);

void skip_whitespace(const char* str, size_t* pos);
void skip_comment(const char*, size_t* pos);
void seek_next_line(const char* str, size_t* pos);
char get_last_interpreted_char(const char* str, const size_t* pos);
void seek_start_of_line(const char* str, size_t* pos);
void seek_end_of_line(const char* str, size_t* pos);

void print_exit_error(const char* msg);
void print_line_information(OP line);

#endif // ASM6502_HELPER_H
