#ifndef ASM6502_LABELS_H
#define ASM6502_LABELS_H

typedef char* LABEL;

LABEL* initialize_table();
LABEL get_label(LABEL*, unsigned char);
int lookup_label(LABEL*, LABEL);
void add_label(LABEL*, LABEL);
void destruct_table(LABEL*);

#endif // ASM6502_LABELS_H
