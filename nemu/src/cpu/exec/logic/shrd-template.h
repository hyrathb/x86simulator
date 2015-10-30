#include "cpu/exec/template-start.h"

#define instr shrd

#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute ()
{
    DATA_TYPE in = op_dest->val;
    DATA_TYPE out = op_src2->val;

    uint8_t count = op_src->val;
    count &= 0x1f;

    cpu.eflags.CF = in >> (count-1) & 0x1;

    while(count != 0)
    {
        out >>= 1;
        out |= (in & 1) << ((DATA_BYTE << 3) - 1);
        in >>= 1;
        count --;
    }

    OPERAND_W(op_src2, out);

#if DATA_BYTE == 1
    cpu.eflags.SF = (out >> 7) & 0x1;
#endif // DATA_BYTE
#if DATA_BYTE == 2
    cpu.eflags.SF = (out >> 15) & 0x1;
#endif // DATA_BYTE
#if DATA_BYTE == 4
    cpu.eflags.SF = (out >> 31) & 0x1;
#endif // DATA_BYTE
    cpu.eflags.ZF = (out == 0);
    cpu.eflags.PF = (out ^ (out >> 1) ^ (out >> 2) ^ (out >> 3) ^ (out >> 4) ^ (out >> 5) ^ (out >> 6) ^ (out >> 7)) & 0x01;

    print_asm("shrd" str(SUFFIX) " %s,%s,%s", op_src->str, op_dest->str, op_src2->str);
}

make_helper(concat(shrdi_, SUFFIX))
{
    int len = concat(decode_si_rm2r_, SUFFIX) (eip + 1);  /* use decode_si_rm2r to read 1 byte immediate */
    op_dest->val = REG(op_dest->reg);
    do_execute();
    return len + 1;
}
#endif

#include "cpu/exec/template-end.h"
