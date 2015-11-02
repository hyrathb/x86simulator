#include "stos.h"
#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"


make_helper(stos_b)
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
    print_asm("stosb");
    return 1;
}

make_helper(stos_w)
{
    swaddr_write(cpu.edi, 2, cpu.ax);
    if (cpu.eflags.DF)
    {
        cpu.edi -= 2;
    }
    else
    {
        cpu.edi += 2;
    }
    print_asm("stosl");
    return 1;
}

make_helper(stos_l)
{
    swaddr_write(cpu.edi, 4, cpu.eax);
    if (cpu.eflags.DF)
    {
        cpu.edi -= 4;
    }
    else
    {
        cpu.edi += 4;
    }
    print_asm("stosl");
    return 1;
}

make_helper_v(stos)
