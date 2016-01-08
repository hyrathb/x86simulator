#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jmp-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */
make_helper_v(jmp_i)
make_helper_v(jmp_rm)

make_helper(ljmp)
{
    uint32_t base = instr_fetch(eip + 1, 4), seg = instr_fetch(eip + 5, 2);
    cpu.eip = base;
    cpu.cs = seg;
    print_asm("jmp 0x%x:0x%x", seg, base);
    return 0;
}
