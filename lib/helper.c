#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#include <unistd.h>
#endif

#include "helper.h"
#include "enums.h"

// TODO: use va_list to pass entire firmat string and additional/optional parameters
void print_exit_error(const char* str) {
  fprintf(stderr, "ERR: %s\n", str);
  exit(EXIT_FAILURE);
}

// honestly this is just here for debug
void print_line_information(OP line) {
  printf("**Information about line:**\n");
  printf("Instruction mnemonic:\t\t%s\n",  (char*)&line.opcode);
  printf("Argument A:\t\t\t%hd\n", line.a.value);
  printf("Argument B:\t\t\t%hd\n", line.b.value);
  printf("**End of instruction information**\n\n");
}

unsigned short int atoh(const char* str, int len) {
#ifdef DEBUG
  printf("Parsing hex number, last digit: %c\n", str[len-1]);
  printf("Whole string: %s\n", str);
#endif
  if (len <= 0) print_exit_error("atoh() called with len <= 0");
  unsigned short int ret = 0;
  unsigned short int mul = 1;
  while (len--) {
    if (isdigit(str[len])) {
      ret += (str[len] - 0x30) * mul;
    } else if (isalpha(str[len])) {
      if (str[len] > 0x40 && str[len] < 0x47) {
        ret += (str[len] - 0x41 + 10) * mul;
      } else {
        print_exit_error("Invalid hexadecimal digit detected.");
      }
    } else {
      print_exit_error("Invalid hexadecimal digit detected.");
    }
    mul *= 0x10;
  }
  return ret;
}

unsigned short int atob(const char* str, int len) {
  unsigned short int ret = 0;
  unsigned short int iterations = 0;
  while (len--) {
    if (str[len] != '1' && str[len] != '0') print_exit_error("Invalid binary digit detected.");
    ret += (str[len] - 0x30) << (iterations++);
  }
  return ret;
}

// note: this is effectively atoi() but with length instead of nullterminator
unsigned short int antoi(const char* str, int len) {
  if (len <= 0) print_exit_error("antoi() called with len <= 0");
  unsigned short int ret = 0;
  unsigned short int mul = 1;
  while (len--) {
    if (isdigit(str[len])) {
      ret += (str[len] - 0x30) * mul;
    } else {
      print_exit_error("Invalid decimal digit detected.");
    }
    mul *= 10;
  }
  return ret;
}

unsigned short int atoreg(const char ch) {
  switch (ch) {
  case A:
    // this one honestly isnt required but its a relic i left in the code
    // since i didnt really know every detail of 6502 asm when i started the project.
    // indeed the register A is only ever used implicitly with the
    // `A` addressing mode.
    print_exit_error("Accumulator is ALWAYS implicitly addressed.");
  case X:
  case Y:
    return (short) ch;
  default:
    print_exit_error("Invalid register name specified.");
  }
  return (short) -1;
}

__attribute__((unused))
int count_lines(const char* str) {
  int count = 0;
  int idx = 0;
  while (str[idx]) {
    if (str[idx] == '\n')
      ++count;
    ++idx;
  }
  return count;
}

// strips whitespace off end of a string
int rstrip(char* str, size_t len) {
  size_t count = 0;
  size_t idx = len - 1;
  while (isspace(str[idx]) && idx > 0) {
    ++count;
    --idx;
  }
  str[idx+1] = '\0';
  return count;
}

// stops @ interpretable character
void skip_whitespace(const char* str, size_t* pos) {
#ifdef DEBUG
  puts("Skipping whitespace...");
#endif
  while (str[*pos] == ' ' || str[*pos] == '\t')
    *pos += 1;

}

// stops at newline/eof
// deprecated in favour of seek_end_of_line()
__attribute__((unused))
void skip_comment(const char* str, size_t* pos) {
  while (str[*pos] != '\n' && str[*pos] != '\0')
    *pos += 1;
}

// skips past newline/stops at eof
void seek_next_line(const char* str, size_t* pos) {
  while (str[*pos] != '\0' && str[*pos] != '\n')
    *pos += 1;
  if (str[*pos] == '\n') *pos += 1;
}

// gets last non-comment/non-whitespace character of a line
char get_last_interpreted_char(const char* str, const size_t* pos) {
  size_t start_idx = *pos;
  size_t last_idx = *pos;
  char flag = 1;

  // Finds last interpreted character
  while (flag) {
    switch (str[last_idx]) {
    case '\n':
    case '\0':
    case ';':
      flag = 0;
      last_idx -= 1;
      break;
    default:
      last_idx += 1;
    }
  }

  // Trims whitespace off the line end if there is any,
  // to find last actual interpreted character.
  while (isspace(str[last_idx]) && start_idx < last_idx) {
    last_idx -= 1;
  }

  return str[last_idx];
}

// goes back to the start of the line
void seek_start_of_line(const char* str, size_t* pos) {
  while (*pos > 0 && str[*pos] != '\n')
    *pos -= 1;

  if (str[*pos] == '\n')
    *pos += 1;

  skip_whitespace(str, pos);
}

// stops at newline/eof
void seek_end_of_line(const char* str, size_t* pos) {
  while (str[*pos] != '\0' && str[*pos] != '\n')
    *pos += 1;
}

void upcase(char* str) {
  size_t idx = 0;
  while (str[idx]) {
    if (isalpha(str[idx]))
      str[idx] &= 0xDF; // only difference between lowercase/uppercase ascii is
                        // one bit, so if i set it to LOW then i effectively
                        // force uppercase
    idx++;
  }
}