#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "ret-template.h"
#undef DATA_BYTE

make_helper(ret)
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
    print_asm("ret");
    return 1;
}
