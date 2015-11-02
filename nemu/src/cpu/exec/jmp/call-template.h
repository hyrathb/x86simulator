#include "cpu/exec/template-start.h"

#define instr call

static void do_execute (int len) {
    cpu.esp -= DATA_BYTE;
    MEM_W(cpu.esp, cpu.eip+len);
    if (op_src->type == OP_TYPE_IMM)
    {
        uint32_t teip = (DATA_TYPE)((DATA_TYPE)cpu.eip + (DATA_TYPE)op_src->simm);
#if DATA_BYTE == 1
        cpu.eip &= 0xffffff00;
        cpu.eip |= teip;
#endif // DATA_BYTE
#if DATA_BYTE == 2 || DATA_BYTE == 4
        cpu.eip = teip;
#endif // DATA_BYTE
    }
    else
        cpu.eip = op_src->val;
#if DATA_BYTE == 2
    cpu.eip &= 0x0000ffff;
#endif // DATA_BYTE

    print_asm_template1();
}

/*
make_instr_helper(i);
make_instr_helper(rm);
*/
make_helper(concat5(instr, _, i, _, SUFFIX))
{

    int len = concat4(decode_, i, _, SUFFIX)(eip+1);
    do_execute(len);
    return len+1;
}

make_helper(concat5(instr, _, rm, _, SUFFIX))
{
    int len = concat4(decode_, rm, _, SUFFIX)(eip+1);
    do_execute(len);
    return 0;
}

#include "cpu/exec/template-end.h"
