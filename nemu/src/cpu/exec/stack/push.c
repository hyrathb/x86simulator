#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "push-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(push_rm)
make_helper_v(push_r)
make_helper_v(push_i)

/*TODO: complete these*/
make_helper(push_cs)
{
    return 1;
}

make_helper(push_ss)
{
    return 1;
}

make_helper(push_ds)
{
    return 1;
}

make_helper(push_es)
{
    return 1;
}

make_helper(push_fs)
{
    return 1;
}

make_helper(push_gs)
{
    return 1;
}

