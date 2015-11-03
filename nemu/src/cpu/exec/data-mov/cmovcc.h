#ifndef CMOVCC_H_INCLUDED
#define CMOVCC_H_INCLUDED

#include "cpu/exec/helper.h"

#define make_cmovcc(name, cond) make_helper(concat3(name, _rm2r_, SUFFIX)) {\
    int len = decode_rm2r_b(eip+1);\
    if (cond)\
    write_operand_b((op_dest), (op_src->val));\
    print_asm(str(name) str(SUFFIX) " %s,%s", op_src->str, op_dest->str);\
    return len+1;\
}

make_helper(cmova_rm2r_v);
make_helper(cmovae_rm2r_v);
make_helper(cmovb_rm2r_v);
make_helper(cmovbe_rm2r_v);
make_helper(cmove_rm2r_v);
make_helper(cmovg_rm2r_v);
make_helper(cmovge_rm2r_v);
make_helper(cmovl_rm2r_v);
make_helper(cmovle_rm2r_v);
make_helper(cmovne_rm2r_v);
make_helper(cmovno_rm2r_v);
make_helper(cmovnp_rm2r_v);
make_helper(cmovns_rm2r_v);
make_helper(cmovo_rm2r_v);
make_helper(cmovp_rm2r_v);
make_helper(cmovs_rm2r_v);

#endif // CMOVCC_H_INCLUDED
