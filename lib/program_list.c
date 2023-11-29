#include "program_list.h"

PROGRAM* initialize_program(void) {
  PROGRAM* ret = (PROGRAM*) malloc(sizeof(PROGRAM));
  ret->length = 0;
  ret->head = NULL;
  ret->tail = NULL;
  return ret;
}

int get_program_length(PROGRAM* p) {
  return p->length;
}

NODE* get_node(PROGRAM* p, int idx) {
  if (idx < 0 || idx >= p->length)
    return NULL;

  NODE* curr = p->head;
  for (int i = 0; i < idx; ++i)
    curr = curr->next;

  return curr;
}

void first_append_operation(PROGRAM* p, OPERATION operation) {
  p->head = (NODE*) malloc(sizeof(NODE));
  p->head->operation = operation;
  p->head->next = NULL;
  p->head->prev = NULL;
  p->tail = p->head;
  p->length++;
}

void append_operation(PROGRAM* p, OPERATION operation) {
  if (p->length == 0) {
    first_append_operation(p, operation);
    return;
  }

  NODE* tmp = (NODE*) malloc(sizeof(NODE));
  tmp->operation = operation;
  tmp->prev = p->tail;
  tmp->next = NULL;

  p->tail->next = tmp;
  p->tail = tmp;
  p->length++;
}

void destruct_program(PROGRAM* p) {
  if (p->length == 0) {
    free(p);
    return;
  }

  NODE* curr = p->tail;
  while (curr->prev) {
    curr = curr->prev;
    free(curr->next);
  }
  free(curr);
  free(p);
}

void traverse(PROGRAM* p, void (*fn)(OPERATION)) {
  if (p->length == 0) {
    puts("List empty. Returning...");
    return;
  }
  NODE* curr = p->head;
  while (curr) {
    fn(curr->operation);
    curr = curr->next;
  }
}