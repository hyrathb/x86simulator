#ifndef SETCC_H_INCLUDED
#define SETCC_H_INCLUDED

#include "cpu/exec/helper.h"

#define make_setcc(name, cond) make_helper(name){\
    int len = decode_rm_b(eip+1);\
    write_operand_b((op_src), (cond));\
    print_asm("%s %s", str(name), op_src->str);\
    return len+1;\
}

make_helper(seta);
make_helper(setae);
make_helper(setb);
make_helper(setbe);
make_helper(sete);
make_helper(setg);
make_helper(setge);
make_helper(setl);
make_helper(setle);
make_helper(setne);
make_helper(setno);
make_helper(setnp);
make_helper(setns);
make_helper(seto);
make_helper(setp);
make_helper(sets);

#endif
