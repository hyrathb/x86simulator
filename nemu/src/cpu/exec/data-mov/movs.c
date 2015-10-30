#include "movs.h"
#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"


make_helper(movsb)
{
    swaddr_write(cpu.edi, 1, swaddr_read(cpu.esi, 1));
    if (cpu.eflags.DF)
    {
        --cpu.edi;
        --cpu.esi;
    }
    else
    {
        ++cpu.edi;
        ++cpu.esi;
    }
    return 1;
}

make_helper(movsv)
{
    int len;
    if (ops_decoded.is_data_size_16)
        len = 2;
    else
        len = 4;
    swaddr_write(cpu.edi, len, swaddr_read(cpu.esi, len));
    if (cpu.eflags.DF)
    {
        cpu.edi -= len;
        cpu.esi -= len;
    }
    else
    {
        cpu.edi += len;
        cpu.esi += len;
    }
    return 1;
}
