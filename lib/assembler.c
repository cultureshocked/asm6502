#include "assembler.h"

void assemble_program(PROGRAM* p, const char* filename) {
  FILE* fp = fopen(filename, "wb");
  if (fp == NULL)
    print_exit_error("Error opening output file. Check folder permissions.");
  if (p == NULL)
    print_exit_error("Lexed program cannot be NULL.");

  size_t program_length = get_program_length(p);
  size_t file_length = 0;

  if (program_length < 1)
    print_exit_error("Program cannot be of length 0.");

  NODE* curr = get_node(p, 0);
  while (curr) {
    OP instruction = curr->operation;
    bytecode bytes = generate_bytecode(instruction);
    file_length += fwrite((void*) &bytes, 1, bytes >> 24, fp);
    curr = curr->next;
  }
  fclose(fp);
  printf("Resulting binary length: %lu\n", file_length);
}