#include "setst.h"
#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(setdf)
{
    cpu.eflags.DF = 1;
    print_asm("std");
    return 1;
}
