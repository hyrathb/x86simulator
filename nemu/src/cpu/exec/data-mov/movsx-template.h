#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute()
{
    DATA_TYPE_S src = (int8_t) op_src->val;
    OPERAND_W(op_dest, src);
    sprintf(op_src->str, "%%%s", regsb[op_src->reg]);

    print_asm_template2();
}

make_instr_helper(rm2r)

#if DATA_BYTE == 4
static void do_movsx_l_w()
{
    DATA_TYPE_S src = (int16_t) op_src->val;
    OPERAND_W(op_dest, src);
    sprintf(op_src->str, "%%%s", regsw[op_src->reg]);

    print_asm_template2();
}


make_helper(movsx_rm2r_l_w)
{
    return idex(eip, concat4(decode_, rm2r, _, SUFFIX), do_movsx_l_w);
}
#endif // DATA_BYTE
#include "cpu/exec/template-end.h"
