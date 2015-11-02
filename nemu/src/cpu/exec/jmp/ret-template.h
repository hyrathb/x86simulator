#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute ()
{
    if (ops_decoded.is_data_size_16)
    {
        cpu.eip = swaddr_read(cpu.esp, 2);
        cpu.esp += 2;
    }
    else
    {
        cpu.eip = swaddr_read(cpu.esp, 4);
        cpu.esp +=4;
    }
    cpu.esp += DATA_BYTE;
    cpu.esp += op_src->val;
    print_asm_template1();
}

make_helper(concat(ret_i_, SUFFIX))
{
    concat(decode_i_, SUFFIX)(eip);
    do_execute();
    return 1;
}


#include "cpu/exec/template-end.h"
