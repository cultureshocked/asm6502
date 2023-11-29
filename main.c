#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helper.h"
#include "program_list.h"
#include "lexer.h"
#include "labels.h"
#include "assembler.h"

#include "instruction_handlers.h"

char* read_file(char*);
void lex(char*, LABEL*, PROGRAM*);
void cleanup(char*, LABEL*, PROGRAM*);

int main(int argc, char** argv) {
  if (argc != 3) {
    printf("usage: %s [input] [output file]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // retrieve all data + allocations
  char* code = read_file(argv[1]);
  LABEL* offset_table = initialize_table();
  PROGRAM* p = initialize_program();

#ifdef DEBUG
  puts("Read file successfully!");
#endif

  // manipulating text to reduce edge cases
  rstrip(code, strlen(code));
  upcase(code);

  // lex
  lex(code, offset_table, p);

#ifdef DEBUG
  puts("Successfully parsed multiline program");
  puts("Traversing symbols...");
  traverse(p, print_line_information);
#endif

  assemble_program(p, argv[2]);

  // finished
  cleanup(code, offset_table, p);
  return 0;
}

char* read_file(char* filename) {
  FILE* fp = fopen(filename, "r");

  if (fp == NULL) {
    fprintf(stderr, "ERR: Could not open file %s\n", filename);
    exit(EXIT_FAILURE);
  }

  // cute way of retrieving file length
  size_t file_length;
  fseek(fp, 0, SEEK_END);
  file_length = ftell(fp);
  rewind(fp);

  // read + close + return
  char* input_data = (char*) calloc(file_length + 1, 1);
  fread(input_data, 1, file_length, fp);
  fclose(fp);
  return input_data;
}



void cleanup(char* code, LABEL* table, PROGRAM* p) {
  free(code);
  destruct_table(table);
  destruct_program(p);
}