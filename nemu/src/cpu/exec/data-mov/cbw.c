#include "cbw.h"
#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper_v(cbw)

make_helper(cbw_w)
{
    if (((int8_t)cpu.al) < (int8_t)0)
        cpu.ah = 0xff;
    else
        cpu.ah = 0;
    print_asm("cbw");
    return 1;
}

make_helper(cbw_l)
{
    int32_t tmp = ((int16_t)cpu.ax);
    cpu.eax = tmp;
    print_asm("cwde");
    return 1;
}
