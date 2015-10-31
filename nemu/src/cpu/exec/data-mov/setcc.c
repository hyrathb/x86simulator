#include "setcc.h"
#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_setcc(seta, !cpu.eflags.CF && !cpu.eflags.ZF)
make_setcc(setae, !cpu.eflags.CF)
make_setcc(setb, cpu.eflags.CF)
make_setcc(setbe, cpu.eflags.CF || cpu.eflags.ZF)
make_setcc(sete, cpu.eflags.ZF)
make_setcc(setg, !cpu.eflags.ZF && (cpu.eflags.SF == cpu.eflags.OF))
make_setcc(setge, cpu.eflags.SF == cpu.eflags.OF)
make_setcc(setl, cpu.eflags.SF != cpu.eflags.OF)
make_setcc(setle, cpu.eflags.ZF || (cpu.eflags.SF != cpu.eflags.OF))
make_setcc(setne, !cpu.eflags.ZF)
make_setcc(setno, !cpu.eflags.OF)
make_setcc(setnp, !cpu.eflags.PF)
make_setcc(setns, !cpu.eflags.SF)
make_setcc(seto, cpu.eflags.OF)
make_setcc(setp, cpu.eflags.PF)
make_setcc(sets, cpu.eflags.SF)

