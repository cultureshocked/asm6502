#ifndef ASM6502_INSTRUCTION_HANDLERS_H
#define ASM6502_INSTRUCTION_HANDLERS_H

#include "enums.h"
#include "helper.h"
#include <stdio.h>

typedef int bytecode;

unsigned char lookup(unsigned int mnemonic);
bytecode generate_bytecode(OP instruction);

bytecode adc_handler(ARGUMENT a, ARGUMENT b);
bytecode and_handler(ARGUMENT a, ARGUMENT b);
bytecode asl_handler(ARGUMENT a, ARGUMENT b);
bytecode bcc_handler(ARGUMENT a, ARGUMENT b);
bytecode bcs_handler(ARGUMENT a, ARGUMENT b);
bytecode beq_handler(ARGUMENT a, ARGUMENT b);
bytecode bit_handler(ARGUMENT a, ARGUMENT b);
bytecode bmi_handler(ARGUMENT a, ARGUMENT b);
bytecode bne_handler(ARGUMENT a, ARGUMENT b);
bytecode bpl_handler(ARGUMENT a, ARGUMENT b);
bytecode brk_handler(ARGUMENT a, ARGUMENT b);
bytecode bvc_handler(ARGUMENT a, ARGUMENT b);
bytecode bvs_handler(ARGUMENT a, ARGUMENT b);
bytecode clc_handler(ARGUMENT a, ARGUMENT b);
bytecode cld_handler(ARGUMENT a, ARGUMENT b);
bytecode cli_handler(ARGUMENT a, ARGUMENT b);
bytecode clv_handler(ARGUMENT a, ARGUMENT b);
bytecode cmp_handler(ARGUMENT a, ARGUMENT b);
bytecode cpx_handler(ARGUMENT a, ARGUMENT b);
bytecode cpy_handler(ARGUMENT a, ARGUMENT b);
bytecode dec_handler(ARGUMENT a, ARGUMENT b);
bytecode dex_handler(ARGUMENT a, ARGUMENT b);
bytecode dey_handler(ARGUMENT a, ARGUMENT b);
bytecode eor_handler(ARGUMENT a, ARGUMENT b);
bytecode inc_handler(ARGUMENT a, ARGUMENT b);
bytecode inx_handler(ARGUMENT a, ARGUMENT b);
bytecode iny_handler(ARGUMENT a, ARGUMENT b);
bytecode jmp_handler(ARGUMENT a, ARGUMENT b);
bytecode jsr_handler(ARGUMENT a, ARGUMENT b);
bytecode lda_handler(ARGUMENT a, ARGUMENT b);
bytecode ldx_handler(ARGUMENT a, ARGUMENT b);
bytecode ldy_handler(ARGUMENT a, ARGUMENT b);
bytecode lsr_handler(ARGUMENT a, ARGUMENT b);
bytecode nop_handler(ARGUMENT a, ARGUMENT b);
bytecode ora_handler(ARGUMENT a, ARGUMENT b);
bytecode pha_handler(ARGUMENT a, ARGUMENT b);
bytecode php_handler(ARGUMENT a, ARGUMENT b);
bytecode pla_handler(ARGUMENT a, ARGUMENT b);
bytecode plp_handler(ARGUMENT a, ARGUMENT b);
bytecode rol_handler(ARGUMENT a, ARGUMENT b);
bytecode ror_handler(ARGUMENT a, ARGUMENT b);
bytecode rti_handler(ARGUMENT a, ARGUMENT b);
bytecode rts_handler(ARGUMENT a, ARGUMENT b);
bytecode sbc_handler(ARGUMENT a, ARGUMENT b);
bytecode sec_handler(ARGUMENT a, ARGUMENT b);
bytecode sed_handler(ARGUMENT a, ARGUMENT b);
bytecode sei_handler(ARGUMENT a, ARGUMENT b);
bytecode sta_handler(ARGUMENT a, ARGUMENT b);
bytecode stx_handler(ARGUMENT a, ARGUMENT b);
bytecode sty_handler(ARGUMENT a, ARGUMENT b);
bytecode tax_handler(ARGUMENT a, ARGUMENT b);
bytecode tay_handler(ARGUMENT a, ARGUMENT b);
bytecode tsx_handler(ARGUMENT a, ARGUMENT b);
bytecode txa_handler(ARGUMENT a, ARGUMENT b);
bytecode txs_handler(ARGUMENT a, ARGUMENT b);
bytecode tya_handler(ARGUMENT a, ARGUMENT b);

#endif // ASM6502_INSTRUCTION_HANDLERS_H
