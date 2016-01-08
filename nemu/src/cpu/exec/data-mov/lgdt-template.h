#include "cpu/exec/template-start.h"
#include "cpu/mmu.h"

#define instr lgdt

static void do_execute() {
	cpu.gdtr.limit = swaddr_read(op_src->addr, 2);
#if DATA_BYTE == 2
    cpu.gdtr.base = swaddr_read(op_src->addr + 2, 3);
#else
    cpu.gdtr.base = swaddr_read(op_src->addr + 2, 4);
#endif // DATA_BYTE
    union
    {
        SegDesc sd;
        struct
        {
            uint32_t _1;
            uint32_t _2;
        };
    } temp;
    temp.sd.base_31_24 = 0;
    temp.sd.base_23_16 = 0;
    temp.sd.base_15_0 = 0;
    temp.sd.limit_19_16 = 0xf;
    temp.sd.limit_15_0 = 0xff;
    swaddr_write(cpu.gdtr.base, 4, temp._1);
    swaddr_write(cpu.gdtr.base + 4, 4, temp._2);
	print_asm_template1();
}

make_instr_helper(rm)
#include "cpu/exec/template-end.h"
