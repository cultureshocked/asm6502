#ifndef ASM6502_PROGRAM_LIST_H
#define ASM6502_PROGRAM_LIST_H

#include <stdio.h>
#include <stdlib.h>

#include "enums.h"

typedef OP OPERATION;

struct list_node {
  struct list_node* next;
  struct list_node* prev;
  OPERATION operation;
};

struct linked_list {
  struct list_node* head;
  struct list_node* tail;
  size_t length;
};

typedef struct list_node NODE;
typedef struct linked_list PROGRAM;

PROGRAM* initialize_program(void);
NODE* get_node(PROGRAM*, int);
int get_program_length(PROGRAM*);
void first_append_operation(PROGRAM*, OPERATION);
void append_operation(PROGRAM*, OPERATION);
void traverse(PROGRAM*, void (*)(OPERATION));
void destruct_program(PROGRAM*);

#endif
