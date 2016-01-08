#include "cpu/exec/helper.h"
#include "cpu/reg.h"

#define DATA_BYTE 1
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "mov-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(mov_i2r)
make_helper_v(mov_i2rm)
make_helper_v(mov_r2rm)
make_helper_v(mov_rm2r)
make_helper_v(mov_a2moffs)
make_helper_v(mov_moffs2a)

make_helper(mov_fromcr)
{
    uint8_t num = instr_fetch(eip+1, 1);
    int gprnum = num & 0x7, crnum = (num >> 3) & 0x7;
    reg_l(gprnum) = cpu.cr[crnum];
    print_asm("mov cr%d, %s", crnum, regsl[gprnum]);
    return 2;
}

make_helper(mov_tocr)
{
    uint8_t num = instr_fetch(eip+1, 1);
    int gprnum = num & 0x7, crnum = (num >> 3) & 0x7;
    cpu.cr[crnum] = reg_l(gprnum);
    print_asm("mov %s, cr%d", regsl[gprnum], crnum);
    return 2;
}

make_helper(mov_toseg)
{
    uint8_t num = instr_fetch(eip+1, 1);
    int gprnum = num & 0x7, segnum = (num >> 3) & 0x7;
    cpu.seg[segnum] = reg_w(gprnum);
    print_asm("mov %s, %s", regsw[gprnum], regseg[segnum]);
    return 2;
}
