#include "instruction_handlers.h"

unsigned char lookup(unsigned int mnemonic) {
  static int offsets[] = {
      LDA, PHA, PLA, ORA, STA, TXA, TYA, SBC, BCC, ADC, DEC, SEC, CLC, INC,
      BVC, SED, CLD, AND, BNE, SEI, CLI, BMI, RTI, BRK, ROL, BPL, ASL, PHP,
      PLP, CMP, JMP, NOP, BEQ, EOR, ROR, JSR, LSR, BCS, RTS, BVS, TXS, BIT,
      CLV, TAX, LDX, DEX, INX, CPX, TSX, STX, TAY, LDY, DEY, INY, CPY, STY,
  };
  unsigned char lower_bound = 0;
  unsigned char upper_bound = 55;

  while (lower_bound <= upper_bound) {
    unsigned char mid = (lower_bound + upper_bound) >> 1;
    if (mnemonic == offsets[mid])
      return ((lower_bound + upper_bound) >> 1);
    if (mnemonic < offsets[mid])
      upper_bound = mid - 1;
    else
      lower_bound = mid + 1;
  }
  print_exit_error("Could not find instruction in the list.");
  return 0;
}

bytecode generate_bytecode(OP instruction) {
  static bytecode (*handlers[56])(ARGUMENT, ARGUMENT) = {
    lda_handler, pha_handler, pla_handler, ora_handler, sta_handler, txa_handler, tya_handler, sbc_handler,
    bcc_handler, adc_handler, dec_handler, sec_handler, clc_handler, inc_handler, bvc_handler, sed_handler,
    cld_handler, and_handler, bne_handler, sei_handler, cli_handler, bmi_handler, rti_handler, brk_handler,
    rol_handler, bpl_handler, asl_handler, php_handler, plp_handler, cmp_handler, jmp_handler, nop_handler,
    beq_handler, eor_handler, ror_handler, jsr_handler, lsr_handler, bcs_handler, rts_handler, bvs_handler,
    txs_handler, bit_handler, clv_handler, tax_handler, ldx_handler, dex_handler, inx_handler, cpx_handler,
    tsx_handler, stx_handler, tay_handler, ldy_handler, dey_handler, iny_handler, cpy_handler, sty_handler
  };
  return handlers[lookup(instruction.opcode)](instruction.a, instruction.b);
}

// Special instructions
bytecode nop_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x010000EA;
}

bytecode brk_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x01000000;
}

// Flag-related instructions
bytecode clv_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x010000B8;
}

bytecode cli_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x01000058;
}

bytecode sei_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x01000078;
}

bytecode cld_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x010000D8;
}

bytecode sed_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x010000F8;
}

bytecode clc_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x01000018;
}

bytecode sec_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x01000038;
}

// Subroutines + Jumps
bytecode rti_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x01000040;
}

bytecode rts_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x01000060;
}

bytecode jsr_handler(const ARGUMENT a, const ARGUMENT b) {
  if (a.type != ADDR)
    print_exit_error("Argument A is not of an address type");
  bytecode ret = 0x0;
  ret |= (bytecode) 0x03 << 24; // length
  ret |= ((bytecode) a.value << 8) & 0xFFFFFF; // argument
  ret |= 0x20; // opcode
  return ret;
}

bytecode jmp_handler(const ARGUMENT a, const ARGUMENT b) {
  if ((a.type & 0xF) != ADDR)
    print_exit_error("Argument A is not of an absolute/indirect absolute type");
  bytecode ret = 0x0;
  ret |= (bytecode) 0x3 << 24;
  ret |= ((bytecode) a.value << 8) & 0xFFFFFF;
  ret |= (a.type & 0x80) ? 0x6C : 0x4C; // (a) or a
  return ret;
}

// Stack instructions
bytecode pha_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x01000048;
}

bytecode pla_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x01000068;
}

bytecode php_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x01000008;
}

bytecode plp_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x01000028;
}


// Transfers/Implicit MOVs
bytecode tax_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x010000AA;
}

bytecode txa_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x0100008A;
}

bytecode tay_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x010000A8;
}

bytecode tya_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x01000098;
}

bytecode tsx_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x010000BA;
}

bytecode txs_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x0100009A;
}

// Branching
// TODO: Add label support
bytecode bcc_handler(const ARGUMENT a, const ARGUMENT b) {
  if (a.type != ADDR || a.value > 0xFF)
    print_exit_error("Argument A is not a valid relative value");
  bytecode ret = 0x0;
  ret |= 0x2 << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= 0x90;
  return ret;
}

bytecode bcs_handler(const ARGUMENT a, const ARGUMENT b) {
  if (a.type != ADDR || a.value > 0xFF)
    print_exit_error("Argument A is not a valid relative value");
  bytecode ret = 0x0;
  ret |= 0x2 << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= 0xB0;
  return ret;
}

bytecode bne_handler(const ARGUMENT a, const ARGUMENT b) {
  if (a.type != ADDR || a.value > 0xFF)
    print_exit_error("Argument A is not a valid relative value");
  bytecode ret = 0x0;
  ret |= 0x2 << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= 0xD0;
  return ret;
}

bytecode beq_handler(const ARGUMENT a, const ARGUMENT b) {
  if (a.type != ADDR || a.value > 0xFF)
    print_exit_error("Argument A is not a valid relative value");
  bytecode ret = 0x0;
  ret |= 0x2 << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= 0xF0;
  return ret;
}

bytecode bpl_handler(const ARGUMENT a, const ARGUMENT b) {
  if (a.type != ADDR || a.value > 0x00FF)
    print_exit_error("Argument A is not a valid relative value");
  bytecode ret = 0x0;
  ret |= (bytecode) 0x2 << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= 0x10;
  return ret;
}

bytecode bmi_handler(const ARGUMENT a, const ARGUMENT b) {
  if (a.type != ADDR || a.value > 0x00FF)
    print_exit_error("Argument A is not a valid relative value");
  bytecode ret = 0x0;
  ret |= 0x2 << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= 0x90;
  return ret;
}

bytecode bvc_handler(const ARGUMENT a, const ARGUMENT b) {
  if (a.type != ADDR || a.value > 0x00FF)
    print_exit_error("Argument A is not a valid relative value");
  bytecode ret = 0x0;
  ret |= 0x2 << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= 0x50;
  return ret;
}

bytecode bvs_handler(const ARGUMENT a, const ARGUMENT b) {
  if (a.type != ADDR || a.value > 0x00FF)
    print_exit_error("Argument A is not a valid relative value");
  bytecode ret = 0x0;
  ret |= 0x2 << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= 0x70;
  return ret;
}

// Increments + Decrements
bytecode inx_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x010000E8;
}

bytecode iny_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x010000C8;
}

bytecode dex_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x010000CA;
}

bytecode dey_handler(const ARGUMENT a, const ARGUMENT b) {
  return 0x01000088;
}

bytecode dec_handler(const ARGUMENT a, const ARGUMENT b) {
  if (a.type != ADDR)
    print_exit_error("DEC: arg1 must be memory type.");
  if (!(b.type == REG && b.value == X) && b.type != NIL)
    print_exit_error("DEC: arg2 must be X register. (optionally nil)");

  bytecode opcode = 0x0;
  bytecode length = 0x0;

  opcode |= (b.type == REG) ? 0xD0 : 0xC0;  //*,x
  opcode |= (a.value > 0xFF) ? 0x0E : 0x06; //a/zp
  length = (a.value > 0xFF) ? 3 : 2;

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= opcode;
  return ret;
}

bytecode inc_handler(const ARGUMENT a, const ARGUMENT b) {
  if (a.type != ADDR)
    print_exit_error("INC: arg1 must be memory type.");
  if (!(b.type == REG && b.value == X) && b.type != NIL)
    print_exit_error("INC: arg2 must be X register. (optionally nil)");

  bytecode opcode = 0x0;
  bytecode length;

  opcode |= (b.type == REG) ? 0xF0 : 0xE0;  //*,x
  opcode |= (a.value > 0xFF) ? 0x0E : 0x06; //a/zp
  length = (a.value > 0xFF) ? 3 : 2;

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= opcode;
  return ret;
}

// Shift/Rotate
bytecode asl_handler(const ARGUMENT a, const ARGUMENT b) {
  if (a.type != ADDR)
    print_exit_error("ASL: arg1 must be memory type.");
  if (!(b.type == REG && b.value == X) && b.type != NIL)
    print_exit_error("ASL: arg2 must be X register. (optionally nil)");

  bytecode opcode = 0x0;
  bytecode length;

  opcode |= (b.type == REG) ? 0x10 : 0x00;  //*,x
  opcode |= (a.value > 0xFF) ? 0x0E : 0x06; //a/zp
  length = (a.value > 0xFF) ? 3 : 2;

  if (a.type == NIL) {
    opcode = 0x0A;
    length = 1;
  }

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8; //does nothing in A mode
  ret |= opcode;
  return ret;
}

bytecode lsr_handler(const ARGUMENT a, const ARGUMENT b) {
  if (a.type != ADDR)
    print_exit_error("LSR: arg1 must be memory type.");
  if (!(b.type == REG && b.value == X) && b.type != NIL)
    print_exit_error("LSR: arg2 must be X register. (optionally nil)");

  bytecode opcode = 0x0;
  bytecode length;

  opcode |= (b.type == REG) ? 0x50 : 0x40;  //*,x
  opcode |= (a.value > 0xFF) ? 0x0E : 0x06; //a/zp
  length = (a.value > 0xFF) ? 3 : 2;

  if (a.type == NIL) {
    opcode = 0x4A;
    length = 1;
  }

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8; //does nothing in A mode
  ret |= opcode;
  return ret;
}

bytecode rol_handler(const ARGUMENT a, const ARGUMENT b) {
  if (a.type != ADDR)
    print_exit_error("ROL: arg1 must be memory type.");
  if (!(b.type == REG && b.value == X) && b.type != NIL)
    print_exit_error("ROL: arg2 must be X register. (optionally nil)");

  bytecode opcode = 0x0;
  bytecode length;

  opcode |= (b.type == REG) ? 0x30 : 0x20;  //*,x
  opcode |= (a.value > 0xFF) ? 0x0E : 0x06; //a/zp
  length = (a.value > 0xFF) ? 3 : 2;

  if (a.type == NIL) {
    opcode = 0x2A;
    length = 1;
  }

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8; //does nothing in A mode
  ret |= opcode;
  return ret;
}

bytecode ror_handler(const ARGUMENT a, const ARGUMENT b) {
  if (a.type != ADDR)
    print_exit_error("ROR: arg1 must be memory type.");
  if (!(b.type == REG && b.value == X) && b.type != NIL)
    print_exit_error("ROR: arg2 must be X register. (optionally nil)");

  bytecode opcode = 0x0;
  bytecode length;

  opcode |= (b.type == REG) ? 0x70 : 0x60;  //*,x
  opcode |= (a.value > 0xFF) ? 0x0E : 0x06; //a/zp
  length = (a.value > 0xFF) ? 3 : 2;

  if (a.type == NIL) {
    opcode = 0x6A;
    length = 1;
  }

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8; //does nothing in A mode
  ret |= opcode;
  return ret;
}

// Comparisons
bytecode cpx_handler(const ARGUMENT a, const ARGUMENT b) {
  if (a.type != ADDR && a.type != NUM)
    print_exit_error("CPX instruction requires a numeric argument.");

  bytecode opcode = 0xE0, length = 3;

  if (a.type == ADDR) {
    opcode |= (a.value > 0xFF) ? 0x0C : 0x04;
    length = (a.value > 0xFF) ? 3 : 2;
  }

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= opcode;
  return ret;
}

bytecode cpy_handler(const ARGUMENT a, const ARGUMENT b) {
  if (a.type != ADDR && a.type != NUM)
    print_exit_error("CPY instruction requires a numeric argument.");

  bytecode opcode = 0xC0, length = 3;

  if (a.type == ADDR) {
    opcode |= (a.value > 0xFF) ? 0x0C : 0x04;
    length = (a.value > 0xFF) ? 3 : 2;
  }

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= opcode;
  return ret;
}

bytecode bit_handler(const ARGUMENT a, const ARGUMENT b) {
  if (a.type != ADDR && a.type != NUM)
    print_exit_error("BIT instruction requires a numeric argument.");

  bytecode opcode = 0x20, length = 3;
  if (a.type == ADDR) {
    opcode |= (a.value > 0xFF) ? 0x0C : 0x04;
    length = (a.value > 0xFF) ? 3 : 2;
  } else
    opcode = 0x89;

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= opcode;
  return ret;
}

bytecode cmp_handler(const ARGUMENT a, const ARGUMENT b) {
  if (((a.type & 0xF) != ADDR && (a.type & 0xF) != NUM) && (b.type != NIL && (b.type & 0xF) != REG))
    print_exit_error("CMP requires: num/addr arg_a, reg/nil arg_b.");
  if (a.type == IADDR && (b.type == REG && b.value != Y))
    print_exit_error("CMP: if arg2 is direct and arg1 is indirect, arg2 must be Y.");

  bytecode opcode = 0, length;
  opcode |= (b.type == REG) ? 0xD0 : 0xC0;

  if (a.value > 0xFF) {
    length = 3;
    opcode |= (b.value == Y) ? 0x09 : 0x0D;
  } else {
    length = 2;
    opcode |= (a.type & 0x80) ? 0x01 : 0x05;
  }

  // Special case numeric
  if ((a.type == NUM)) {
    opcode = 0xC9;
    length = 3;
  }

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= opcode;
  return ret;
}

// Load/Store
bytecode ldx_handler(const ARGUMENT a, const ARGUMENT b) {
  if ((a.type != ADDR && a.type != NUM) || (b.type != NIL && b.type != REG))
    print_exit_error("Invalid arguments passed to LDX.");
  if (b.type == REG && b.value != Y)
    print_exit_error("LDX only supports 2nd argument Y.");

  bytecode opcode = 0, length = 0;
  opcode |= (b.type == REG) ? 0xB0 : 0xA0;    //y-mode or no
  if (a.type == ADDR) {
    opcode |= (a.value > 0xFF) ? 0x0E : 0x06; //zp/a
    length = (a.value > 0xFF) ? 3 : 2;
  } else {
    opcode |= 0x02;                           //numeric literal, 0xA0 set from y-mode check
    length = 3;
  }

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= opcode;
  return ret;
}

bytecode ldy_handler(const ARGUMENT a, const ARGUMENT b) {
  if ((a.type != ADDR && a.type != NUM) || (b.type != NIL && b.type != REG))
    print_exit_error("Invalid arguments passed to LDY.");
  if (b.type == REG && b.value != X)
    print_exit_error("LDY only supports 2nd argument X.");

  bytecode opcode = 0, length;
  opcode |= (b.type == REG) ? 0xB0 : 0xA0;    //y-mode or no
  if (a.type == ADDR) {
    opcode |= (a.value > 0xFF) ? 0x0C : 0x04; //zp/a
    length = (a.value > 0xFF) ? 3 : 2;
  } else {     //numeric literal, 0xA0 set from ymode check
    length = 3;
  }

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= opcode;
  return ret;
}

bytecode sty_handler(const ARGUMENT a, const ARGUMENT b) {
  if ((a.type != ADDR) || (b.type != NIL && b.type != REG))
    print_exit_error("Invalid arguments supplied to STY.");
  if (b.type == REG && b.value != X)
    print_exit_error("STY only supports arg2 as register X");

  bytecode opcode = 0, length;
  opcode |= (b.type == REG) ? 0x90 : 0x80;
  if (a.value > 0xFF) {
    length = 3;
    opcode = 0x8C;
  } else {
    length = 2;
    opcode |= 0x04;
  }

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= opcode;
  return ret;
}

bytecode stx_handler(const ARGUMENT a, const ARGUMENT b) {
  if ((a.type != ADDR) || (b.type != NIL && b.type != REG))
    print_exit_error("Invalid arguments supplied to STX.");
  if (b.type == REG && b.value != Y)
    print_exit_error("STX only supports arg2 as register Y");

  bytecode opcode = 0, length;
  opcode |= (b.type == REG) ? 0x90 : 0x80;
  if (a.value > 0xFF) {
    length = 3;
    opcode = 0x8E;
  } else {
    length = 2;
    opcode |= 0x06;
  }

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= opcode;
  return ret;
}

bytecode lda_handler(const ARGUMENT a, const ARGUMENT b) {
  if (((a.type & 0xF) != ADDR && (a.type & 0xF) != NUM) && (b.type != NIL && (b.type & 0xF) != REG))
    print_exit_error("LDA requires: num/addr arg_a, reg/nil arg_b.");
  if (a.type == IADDR && (b.type == REG && b.value != Y))
    print_exit_error("LDA: if arg2 is direct and arg1 is indirect, arg2 must be Y.");

  bytecode opcode = 0, length;
  opcode |= (b.type == REG) ? 0xB0 : 0xA0;

  if (a.value > 0xFF) {
    length = 3;
    opcode |= (b.value == Y) ? 0x09 : 0x0D;
  } else {
    length = 2;
    opcode |= (a.type & 0x80) ? 0x01 : 0x05;
  }

  // Special case numeric
  if ((a.type == NUM)) {
    opcode = 0xA9;
    length = 3;
  }

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= opcode;
  return ret;
}

bytecode sta_handler(const ARGUMENT a, const ARGUMENT b) {
  if ((a.type & 0xF) != ADDR && (b.type != NIL && (b.type & 0xF) != REG))
    print_exit_error("STA requires: addr arg_a, reg/nil arg_b.");
  if (a.type == IADDR && (b.type == REG && b.value != Y))
    print_exit_error("STA: If arg2 is direct and arg1 is indirect, arg2 must be Y.");
  if (a.type == IADDR && (b.type == IREG && b.value != X))
    print_exit_error("STA: If both args are indirect, arg2 = X only.");

  bytecode opcode = 0, length;
  opcode |= (b.type == REG) ? 0x90 : 0x80; // *,x/y

  if (a.value > 0xFF) { // a,*
    length = 3;
    opcode |= (b.value == Y) ? 0x09 : 0x0D; // a,y/x
  } else {  //zp,*
    length = 2;
    opcode |= (a.type & 0x80) ? 0x01 : 0x05;// (zp)/zp,*
  }

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= opcode;
  return ret;
}

// Arithmetic (Add/Sub)
bytecode adc_handler(const ARGUMENT a, const ARGUMENT b) {
  if (((a.type & 0xF) != ADDR && (a.type & 0xF) != NUM) && (b.type != NIL && (b.type & 0xF) != REG))
    print_exit_error("ADC requires: num/addr arg_a, reg/nil arg_b.");
  if (a.type == IADDR && (b.type == REG && b.value != Y))
    print_exit_error("ADC: if arg2 is direct and arg1 is indirect, arg2 must be Y.");
  if (a.type == NUM && a.value > 0xFF)
    print_exit_error("Immediate value must be 8bit.");

  bytecode opcode = 0, length;
  opcode |= (b.type == REG) ? 0x70 : 0x60;

  if (a.value > 0xFF) {
    length = 3;
    opcode |= (b.value == Y) ? 0x09 : 0x0D;
  } else {
    length = 2;
    opcode |= (a.type & 0x80) ? 0x01 : 0x05;
  }

  // Special case numeric
  if (a.type == NUM) {
    opcode = 0x69;
    length = 2;
  }

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= opcode;
  return ret;
}

bytecode sbc_handler(const ARGUMENT a, const ARGUMENT b) {
  if (((a.type & 0xF) != ADDR && (a.type & 0xF) != NUM) && (b.type != NIL && (b.type & 0xF) != REG))
    print_exit_error("SBC requires: num/addr arg_a, reg/nil arg_b.");
  if (a.type == IADDR && (b.type == REG && b.value != Y))
    print_exit_error("SBC: if arg2 is direct and arg1 is indirect, arg2 must be Y.");
  if (a.type == NUM && a.value > 0xFF)
    print_exit_error("Immediate value must be 8bit.");

  bytecode opcode = 0, length;
  opcode |= (b.type == REG) ? 0xF0 : 0xE0;

  if (a.value > 0xFF) {
    length = 3;
    opcode |= (b.value == Y) ? 0x09 : 0x0D;
  } else {
    length = 2;
    opcode |= (a.type & 0x80) ? 0x01 : 0x05;
  }

  // Special case numeric
  if ((a.type == NUM)) {
    opcode = 0x29;
    length = 2;
  }

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= opcode;
  return ret;
}

// Logic/Bitwise
bytecode and_handler(const ARGUMENT a, const ARGUMENT b) {
  if (((a.type & 0xF) != ADDR && (a.type & 0xF) != NUM) && (b.type != NIL && (b.type & 0xF) != REG))
    print_exit_error("AND requires: num/addr arg_a, reg/nil arg_b.");
  if (a.type == IADDR && (b.type == REG && b.value != Y))
    print_exit_error("AND: if arg2 is direct and arg1 is indirect, arg2 must be Y.");
  if (a.type == NUM && a.value > 0xFF)
    print_exit_error("Immediate value must be 8bit.");

  bytecode opcode = 0, length;
  opcode |= (b.type == REG) ? 0x30 : 0x20;

  if (a.value > 0xFF) {
    length = 3;
    opcode |= (b.value == Y) ? 0x09 : 0x0D;
  } else {
    length = 2;
    opcode |= (a.type & 0x80) ? 0x01 : 0x05;
  }

  // Special case numeric
  if ((a.type == NUM)) {
    opcode = 0x29;
    length = 2;
  }

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= opcode;
  return ret;
}

bytecode ora_handler(const ARGUMENT a, const ARGUMENT b) {
  if (((a.type & 0xF) != ADDR && (a.type & 0xF) != NUM) && (b.type != NIL && (b.type & 0xF) != REG))
    print_exit_error("ORA requires: num/addr arg_a, reg/nil arg_b.");
  if (a.type == IADDR && (b.type == REG && b.value != Y))
    print_exit_error("ORA: if arg2 is direct and arg1 is indirect, arg2 must be Y.");
  if (a.type == NUM && a.value > 0xFF)
    print_exit_error("Immediate value must be 8bit.");

  bytecode opcode = 0, length;
  opcode |= (b.type == REG) ? 0x10 : 0x00;

  if (a.value > 0xFF) {
    length = 3;
    opcode |= (b.value == Y) ? 0x09 : 0x0D;
  } else {
    length = 2;
    opcode |= (a.type & 0x80) ? 0x01 : 0x05;
  }

  // Special case numeric
  if ((a.type == NUM)) {
    opcode = 0x09;
    length = 2;
  }

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= opcode;
  return ret;
}

bytecode eor_handler(const ARGUMENT a, const ARGUMENT b) {
  if (((a.type & 0xF) != ADDR && (a.type & 0xF) != NUM) && (b.type != NIL && (b.type & 0xF) != REG))
    print_exit_error("EOR requires: num/addr arg_a, reg/nil arg_b.");
  if (a.type == IADDR && (b.type == REG && b.value != Y))
    print_exit_error("EOR: if arg2 is direct and arg1 is indirect, arg2 must be Y.");
  if (a.type == NUM && a.value > 0xFF)
    print_exit_error("Immediate value must be 8bit.");

  bytecode opcode = 0, length;
  opcode |= (b.type == REG) ? 0x50 : 0x40;

  if (a.value > 0xFF) {
    length = 3;
    opcode |= (b.value == Y) ? 0x09 : 0x0D;
  } else {
    length = 2;
    opcode |= (a.type & 0x80) ? 0x01 : 0x05;
  }

  // Special case numeric
  if ((a.type == NUM)) {
    opcode = 0x49;
    length = 2;
  }

  bytecode ret = 0x0;
  ret |= length << 24;
  ret |= ((bytecode) a.value & 0xFFFF) << 8;
  ret |= opcode;
  return ret;
}

// DONE!!!