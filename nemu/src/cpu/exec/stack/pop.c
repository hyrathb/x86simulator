#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "pop-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "pop-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(pop_rm)
make_helper_v(pop_r)

/*TODO: complete these*/
make_helper(pop_cs)
{
    return 1;
}

make_helper(pop_ss)
{
    return 1;
}

make_helper(pop_ds)
{
    return 1;
}

make_helper(pop_es)
{
    return 1;
}

make_helper(pop_fs)
{
    return 1;
}

make_helper(pop_gs)
{
    return 1;
}

