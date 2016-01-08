#include "cpu/exec/template-start.h"

#define instr push

static void do_execute()
{
#if DATA_BYTE == 2
    cpu.esp -= 2;
    cpu.current_sreg = SS;
    swaddr_write(cpu.esp, 2, op_src->val);
#else
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, op_src->val);
#endif // DATA_BYTE
    print_asm_template1();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(rm)
make_instr_helper(r)
#endif // DATA_BYTE
make_instr_helper(i)


#include "cpu/exec/template-end.h"
