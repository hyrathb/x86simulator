#include "cmovcc.h"
#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

#define DATA_BYTE 4
#define DATA_TYPE uint32_t
#define SUFFIX l
make_cmovcc(cmova, !cpu.eflags.CF && !cpu.eflags.ZF)
make_cmovcc(cmovae, !cpu.eflags.CF)
make_cmovcc(cmovb, cpu.eflags.CF)
make_cmovcc(cmovbe, cpu.eflags.CF || cpu.eflags.ZF)
make_cmovcc(cmove, cpu.eflags.ZF)
make_cmovcc(cmovg, !cpu.eflags.ZF && (cpu.eflags.SF == cpu.eflags.OF))
make_cmovcc(cmovge, cpu.eflags.SF == cpu.eflags.OF)
make_cmovcc(cmovl, cpu.eflags.SF != cpu.eflags.OF)
make_cmovcc(cmovle, cpu.eflags.ZF || (cpu.eflags.SF != cpu.eflags.OF))
make_cmovcc(cmovne, !cpu.eflags.ZF)
make_cmovcc(cmovno, !cpu.eflags.OF)
make_cmovcc(cmovnp, !cpu.eflags.PF)
make_cmovcc(cmovns, !cpu.eflags.SF)
make_cmovcc(cmovo, cpu.eflags.OF)
make_cmovcc(cmovp, cpu.eflags.PF)
make_cmovcc(cmovs, cpu.eflags.SF)
#undef DATA_BYTE
#undef DATA_TYPE
#undef SUFFIX

#define DATA_BYTE 2
#define DATA_TYPE uint16_t
#define SUFFIX w
make_cmovcc(cmova, !cpu.eflags.CF && !cpu.eflags.ZF)
make_cmovcc(cmovae, !cpu.eflags.CF)
make_cmovcc(cmovb, cpu.eflags.CF)
make_cmovcc(cmovbe, cpu.eflags.CF || cpu.eflags.ZF)
make_cmovcc(cmove, cpu.eflags.ZF)
make_cmovcc(cmovg, !cpu.eflags.ZF && (cpu.eflags.SF == cpu.eflags.OF))
make_cmovcc(cmovge, cpu.eflags.SF == cpu.eflags.OF)
make_cmovcc(cmovl, cpu.eflags.SF != cpu.eflags.OF)
make_cmovcc(cmovle, cpu.eflags.ZF || (cpu.eflags.SF != cpu.eflags.OF))
make_cmovcc(cmovne, !cpu.eflags.ZF)
make_cmovcc(cmovno, !cpu.eflags.OF)
make_cmovcc(cmovnp, !cpu.eflags.PF)
make_cmovcc(cmovns, !cpu.eflags.SF)
make_cmovcc(cmovo, cpu.eflags.OF)
make_cmovcc(cmovp, cpu.eflags.PF)
make_cmovcc(cmovs, cpu.eflags.SF)
#undef DATA_BYTE
#undef DATA_TYPE
#undef SUFFIX

make_helper_v(cmova_rm2r)
make_helper_v(cmovae_rm2r)
make_helper_v(cmovb_rm2r)
make_helper_v(cmovbe_rm2r)
make_helper_v(cmove_rm2r)
make_helper_v(cmovg_rm2r)
make_helper_v(cmovge_rm2r)
make_helper_v(cmovl_rm2r)
make_helper_v(cmovle_rm2r)
make_helper_v(cmovne_rm2r)
make_helper_v(cmovno_rm2r)
make_helper_v(cmovnp_rm2r)
make_helper_v(cmovns_rm2r)
make_helper_v(cmovo_rm2r)
make_helper_v(cmovp_rm2r)
make_helper_v(cmovs_rm2r)
