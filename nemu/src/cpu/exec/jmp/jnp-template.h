#include "cpu/exec/template-start.h"

#define instr jnp

static void do_execute ()
{
    if (!cpu.eflags.PF)
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
    print_asm_template1();
}

make_instr_helper(i)


#include "cpu/exec/template-end.h"
