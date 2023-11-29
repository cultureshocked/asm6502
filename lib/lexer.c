#include "lexer.h"
#include "helper.h"

void lex(char* code, LABEL* offset_table, PROGRAM* symbolic_code) {
  size_t position = 0;
  size_t code_length = strlen(code);

  while (position < code_length) {
    if (code[position] == '\0')
      break;
    if (code[position] == '\n') {
      seek_next_line(code, &position);
      continue;
    }
    if (code[position] == ' ' || code[position] == '\t') {
      skip_whitespace(code, &position);
      continue;
    }

    OP op = parse_line(code, &position);

    //Label parsing
    if (op.opcode == 0xFFFFFFFF) {
      seek_start_of_line(code, &position);
      char label_buffer[32] = {0};
      int label_ptr = 0;
      while (code[position + label_ptr] != ':') {
        label_ptr++;
      }

      if (label_ptr >= 32)
        print_exit_error("Label too long (max. 31 chars).");
      if (lookup_label(offset_table, label_buffer) != -1)
        print_exit_error("Redeclared/duplicate label found.");

      printf("Label ptr is %d\n", label_ptr);
      strncpy(label_buffer, code+position, label_ptr);
      printf("label buffer is: %s\nremaining code is %s", label_buffer, code+position);
      add_label(offset_table, label_buffer);
    } // End label parsing

    append_operation(symbolic_code, op);
    seek_next_line(code, &position);
  }
}

int parse_instruction(const char* str, size_t* pos) {
#ifdef DEBUG
  puts("Parsing instruction mnemonic...");
  //printf("Passed string is: %s with position %lu\n", str, *pos);
  printf("Holds value %s\n", str+(*pos));
  printf("characters: %c (%hhx), %c (%hhx), %c (%hhx)\n", str[*pos], str[*pos], str[*pos + 1], str[*pos + 1], str[*pos + 2], str[*pos + 2]);
#endif
  char buf[4] = {0};
  strncpy(buf, str+(*pos), 3);
  int tkn = * (int*) buf;
  switch (tkn) {
  case ADC:
  case AND:
  case ASL:
  case BCC:
  case BCS:
  case BEQ:
  case BIT:
  case BMI:
  case BNE:
  case BPL:
  case BRK:
  case BVC:
  case BVS:
  case CLC:
  case CLD:
  case CLI:
  case CLV:
  case CMP:
  case CPX:
  case CPY:
  case DEC:
  case DEX:
  case DEY:
  case EOR:
  case INC:
  case INX:
  case INY:
  case JMP:
  case JSR:
  case LDA:
  case LDX:
  case LDY:
  case LSR:
  case NOP:
  case ORA:
  case PHA:
  case PHP:
  case PLA:
  case PLP:
  case ROL:
  case ROR:
  case RTI:
  case RTS:
  case SBC:
  case SEC:
  case SED:
  case SEI:
  case STA:
  case STX:
  case STY:
  case TAX:
  case TAY:
  case TSX:
  case TXA:
  case TXS:
  case TYA:
#ifdef DEBUG
    printf("Returning mnemonic token: %s\n", buf);
#endif
    *pos += 3;
    return tkn;
  default:
    print_exit_error("Invalid instruction mnemonic detected.");
  }
  return -1;
}

unsigned short int parse_numeric(const char* str, size_t* pos) {
#ifdef DEBUG
  puts("Parsing numeric argument...");
  printf("Supplied argument: %4s...\n", str+(*pos));
#endif
  int seek = 0;
  unsigned short int ret = 0;
  while (isdigit(str[(*pos) + seek]) ||
         str[(*pos) + seek] == '$' ||
         str[(*pos) + seek] == '%' ||
         (str[(*pos) + seek] > 0x40 && str[(*pos) + seek] < 0x47)) {
    ++seek;
  }
  switch (str[*pos]) {
  case '$':
    if (seek == 1)
      print_exit_error("Hex prefix with no numeric portion detected.");
    ret = atoh(str+(*pos)+1, seek-1);
    break;
  case '%':
    if (seek == 1)
      print_exit_error("Binary prefix with no numeric portion detected.");
    ret = atob(str+(*pos)+1, seek-1);
    break;
  default:
    if (isdigit(str[*pos]))
      ret = antoi(str+(*pos), seek);
    else
      print_exit_error("Invalid numeric character found.");
  }
  *pos += seek;
  return ret;
}

unsigned short int parse_register(const char* str, size_t* pos) {
#ifdef DEBUG
  puts("Parsing register...");
#endif
  unsigned short int ret = atoreg(str[*pos]);
  *pos += 1;
  return ret;
}

ARGUMENT parse_argument(const char* str, size_t* pos) {
#ifdef DEBUG
  puts("Parsing argument...");
#endif
  ARGUMENT ret = (ARGUMENT) {.type=NIL, .value=0};
  if (str == NULL) return ret;
  switch (str[*pos]) {
  case '#':
    *pos += 1;
    ret.value = parse_numeric(str, pos);
    ret.type = NUM;
    break;
  default:
    if (isalpha(str[*pos])) {
      ret.type = REG;
      ret.value = parse_register(str, pos);
    } else if (str[*pos] == '$' || str[*pos] == '%') {
      ret.value = parse_numeric(str, pos);
      ret.type = ADDR;
    } else {
      if (isdigit(str[*pos])) {
        ret.type = ADDR;
        ret.value = parse_numeric(str, pos);
      } else {
        print_exit_error("Invalid character at start of argument.");
      }
    }
  }
  return ret;
}

// i'm really sorry if you're trying to read this for fun.
// it looks like if/else hell but in reality 6 out of the 9 are for error messages.
// i can accept that level of unreadability.

// TODO: add label argument support
// TODO: abstract label-line parsing
// TODO: fully deprecate OP.mode field
// TODO: add `define` support for variables
OP parse_line(const char* str, size_t* pos) {
  static unsigned char label_count = 0;

#ifdef DEBUG
  printf("Parsing line: ");
  size_t count = *pos;

  do {
    if (isalnum(str[count]))
      putchar(str[count]);
    else
      printf("[%hhx]", str[count]);
    count++;
  } while (str[count] != '\n' && str[count] != '\0');
  putchar('\n');
#endif

  OP ret;

  // label line
  if (get_last_interpreted_char(str, pos) == ':') {
    ret.opcode = 0xFFFFFFFF;
    ret.a.value = label_count;
    ret.a.type = LL;
    ret.b.value = 0;
    ret.b.type = 0;
    ret.mode = 0;
    label_count++;
    seek_end_of_line(str, pos);
    *pos -= 1;
    return ret;
  }

  //regular line
  ret.opcode = parse_instruction(str, pos);

  unsigned int arg_count = 0;
  unsigned int break_flag = 0;
  unsigned char is_indirect = 0;

  while (str[*pos] != '\n' && str[*pos] != '\0' && break_flag == 0) {
#ifdef DEBUG
    printf("Character at position %lu: %c\n", *pos, str[*pos]);
#endif
    switch (str[*pos]) {
    case ',':
      *pos += 1;
      continue;
    case ' ':
    case '\t':
      skip_whitespace(str, pos);
      continue;
    case '(':
      if (is_indirect)
        print_exit_error("Cannot interpret nested indirection.");
      is_indirect = 1;
      *pos += 1;
      continue;
    case ')':
      if (!is_indirect)
        print_exit_error("Currently processed argument is already direct.");
      is_indirect = 0;
      *pos += 1;
      continue;
    case ';':
    case '\n':
      break_flag = 1;
      break;
    default:
      if (arg_count >= 2)
        print_exit_error("Too many arguments for a single instruction");
      if (arg_count) {
        ret.b = parse_argument(str, pos);
        if (is_indirect) {
          if (ret.b.type != REG || ret.b.value != X)
            print_exit_error("The only possibly indirect second argument is the register X.");
          ret.b.type &= 0x80;
        }
        arg_count++;
        break_flag = 1;
      } else {
        ret.a = parse_argument(str, pos);
        if (is_indirect) {
          if (ret.a.type != ADDR || ret.a.value > 0xFF)
            print_exit_error("Only zeropaged memory can be indirect.");
          ret.a.type &= 0x80;
        }
        arg_count++;
      }
    }
  }
  if (arg_count == 0) ret.a = parse_argument(NULL, 0);
  if (arg_count < 2) ret.b = parse_argument(NULL, 0);
  if (is_indirect)
    print_exit_error("Indirection parenthesis has not been closed.");
  ret.mode = 0;
  return ret;
}