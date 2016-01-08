#include "cmps.h"
#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(cmps_b)
{
    uint8_t src, dst;
    cpu.current_sreg = 0;
    src = swaddr_read(cpu.edi, 1);
    cpu.current_sreg = 3;
    dst = swaddr_read(cpu.esi, 1);
    uint8_t result = dst - src;

    cpu.eflags.CF = ((uint32_t)dst < (uint32_t)src);
    cpu.eflags.PF = (result ^ (result >> 1) ^ (result >> 2) ^ (result >> 3) ^ (result >> 4) ^ (result >> 5) ^ (result >> 6) ^ (result >> 7)) & 0x01;
    cpu.eflags.AF = ((uint32_t)(dst & 0x0f) < (uint32_t)(src & 0x0f));
    cpu.eflags.ZF = (result == 0);
#if DATA_BYTE == 1
    cpu.eflags.SF = (result >> 7) & 0x1;
#endif // DATA_BYTE
#if DATA_BYTE == 2
    cpu.eflags.SF = (result >> 15) & 0x1;
#endif // DATA_BYTE
#if DATA_BYTE == 4
    cpu.eflags.SF = (result >> 31) & 0x1;
#endif // DATA_BYTE
    cpu.eflags.OF = (((int32_t)(dst) >= 0) && ((int32_t)(src) < 0) && ((int32_t)result < 0)) || (((int32_t)(dst) <= 0) && ((int32_t)(src) > 0) && ((int32_t)result > 0));

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
    print_asm("cmpsb");
    return 1;
}

make_helper(cmps_w)
{
    uint16_t src, dst;
    cpu.current_sreg = 0;
    src = swaddr_read(cpu.di, 2);
    cpu.current_sreg = 3;
    dst = swaddr_read(cpu.si, 2);
    uint16_t result = dst - src;

    cpu.eflags.CF = ((uint32_t)dst < (uint32_t)src);
    cpu.eflags.PF = (result ^ (result >> 1) ^ (result >> 2) ^ (result >> 3) ^ (result >> 4) ^ (result >> 5) ^ (result >> 6) ^ (result >> 7)) & 0x01;
    cpu.eflags.AF = ((uint32_t)(dst & 0x0f) < (uint32_t)(src & 0x0f));
    cpu.eflags.ZF = (result == 0);
#if DATA_BYTE == 1
    cpu.eflags.SF = (result >> 7) & 0x1;
#endif // DATA_BYTE
#if DATA_BYTE == 2
    cpu.eflags.SF = (result >> 15) & 0x1;
#endif // DATA_BYTE
#if DATA_BYTE == 4
    cpu.eflags.SF = (result >> 31) & 0x1;
#endif // DATA_BYTE
    cpu.eflags.OF = (((int32_t)(dst) >= 0) && ((int32_t)(src) < 0) && ((int32_t)result < 0)) || (((int32_t)(dst) <= 0) && ((int32_t)(src) > 0) && ((int32_t)result > 0));

    if (cpu.eflags.DF)
    {
        cpu.di -= 2;
        cpu.si -=2;
    }
    else
    {
        cpu.di += 2;
        cpu.si += 2;
    }
    print_asm("cmpsw");
    return 1;
}

make_helper(cmps_l)
{
    uint32_t src, dst;
    cpu.current_sreg = 0;
    src = swaddr_read(cpu.edi, 1);
    cpu.current_sreg = 3;
    dst = swaddr_read(cpu.esi, 1);
    uint32_t result = dst - src;

    cpu.eflags.CF = ((uint32_t)dst < (uint32_t)src);
    cpu.eflags.PF = (result ^ (result >> 1) ^ (result >> 2) ^ (result >> 3) ^ (result >> 4) ^ (result >> 5) ^ (result >> 6) ^ (result >> 7)) & 0x01;
    cpu.eflags.AF = ((uint32_t)(dst & 0x0f) < (uint32_t)(src & 0x0f));
    cpu.eflags.ZF = (result == 0);
#if DATA_BYTE == 1
    cpu.eflags.SF = (result >> 7) & 0x1;
#endif // DATA_BYTE
#if DATA_BYTE == 2
    cpu.eflags.SF = (result >> 15) & 0x1;
#endif // DATA_BYTE
#if DATA_BYTE == 4
    cpu.eflags.SF = (result >> 31) & 0x1;
#endif // DATA_BYTE
    cpu.eflags.OF = (((int32_t)(dst) >= 0) && ((int32_t)(src) < 0) && ((int32_t)result < 0)) || (((int32_t)(dst) <= 0) && ((int32_t)(src) > 0) && ((int32_t)result > 0));

    if (cpu.eflags.DF)
    {
        cpu.edi -= 4;
        cpu.esi -= 4;
    }
    else
    {
        cpu.edi += 4;
        cpu.esi += 4;
    }
    print_asm("cmpsl");
    return 1;
}

make_helper_v(cmps)
