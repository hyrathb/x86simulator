#include "stos.h"
#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"


make_helper(stosb)
{
    swaddr_write(cpu.edi, 1, cpu.al);
    if (cpu.eflags.DF)
    {
        --cpu.edi;
    }
    else
    {
        ++cpu.edi;
    }
    return 1;
}

make_helper(stosv)
{
    uint32_t src;
    int len;
    if (ops_decoded.is_data_size_16)
    {
        src = cpu.ax;
        len = 2;
    }
    else
    {
        src = cpu.eax;
        len = 4;
    }
    swaddr_write(cpu.edi, len, src);
    if (cpu.eflags.DF)
    {
        cpu.edi -= len;
    }
    else
    {
        cpu.edi += len;
    }
    return 1;
}
