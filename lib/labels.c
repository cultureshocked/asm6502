#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "labels.h"

LABEL* initialize_table() {
  LABEL* ret = (LABEL*) calloc(sizeof(LABEL), 256);
  for (int i = 0; i < 256; ++i)
    ret[i] = NULL;
  return ret;
}

LABEL get_label(LABEL* table, unsigned char idx) {
  return table[idx];
}

int lookup_label(LABEL* table, LABEL str) {
  for (int i = 0; i < 256; ++i) {
    if (table[i] == NULL) break;
    if (strcmp(table[i], str) == 0)
      return i;
  }
  return -1;
}

void destruct_table(LABEL* table) {
  for (int i = 0; i < 256; ++i) {
    if (table[i] == NULL)
      break;
    free(table[i]);
  }
  free(table);
}

void add_label(LABEL* table, LABEL str) {
  static unsigned char idx = 0;

  table[idx] = (char*) malloc(strlen(str) + 1);

}

void print_table(LABEL* table) {
  for (int i = 0; i < 256; ++i) {
    if (table[i] == NULL) break;
    printf("Label: %s\n", table[i]);
  }
}