#include "movs.h"
#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"


make_helper(movsb)
{
    cpu.current_sreg = 3;
    uint8_t result = swaddr_read(cpu.esi, 1);
    cpu.current_sreg = 0;
    swaddr_write(cpu.edi, 1, result);
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
    cpu.current_sreg = 3;
    uint32_t result = swaddr_read(cpu.esi, len);
    cpu.current_sreg = 0;
    swaddr_write(cpu.edi, len, result);
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
