#include "cpu/exec/helper.h"
#include "leave.h"

make_helper(leave)
{
    if (ops_decoded.is_data_size_16)
    {
        cpu.sp = cpu.bp;
        cpu.bp = swaddr_read(cpu.sp, 2);
        cpu.sp += 2;
    }
    else
    {
        cpu.esp = cpu.ebp;
        cpu.ebp = swaddr_read(cpu.esp, 4);
        cpu.esp += 4;
    }
    print_asm("leave");
    return 1;
}
