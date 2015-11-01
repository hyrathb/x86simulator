#include "cpu/exec/template-start.h"

#define instr jb

static void do_execute ()
{
    if (cpu.eflags.CF)
    {
        int32_t tsrc = (DATA_TYPE_S)op_src->val;
        cpu.eip += tsrc;
#if DATA_BYTE == 2
        cpu.eip &= 0x0000ffff;
#endif // DATA_BYTE
    }
    print_asm_template1();
}

make_instr_helper(i)


#include "cpu/exec/template-end.h"
