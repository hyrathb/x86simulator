#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
    cpu.esp -= DATA_BYTE;
	MEM_W(cpu.esp, op_src->val);
	print_asm_template1();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(rm)
make_instr_helper(r)
#endif // DATA_BYTE
make_instr_helper(i)


#include "cpu/exec/template-end.h"
