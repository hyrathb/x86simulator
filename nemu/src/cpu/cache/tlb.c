#include "cpu/cache/tlb.h"
#include "memory/memory.h"
#include "cpu/reg.h"

tlb cpu_tlb;

uint32_t tlb_lookup(uint32_t addr)
{
    static int num = 0;
    int i, last_num = -1;
    hwaddr_t phyaddr;
    tlbaddr tmpaddr;
    tmpaddr.addr = addr;
    ++cpu_tlb.req;
    for (i=0; i<TLB_ENTRIES; ++i)
        if (cpu_tlb.entries[i].valid)
        {
            if (cpu_tlb.entries[i].tag == ((addr & TLB_TAG_MASK) >> 12))
                break;
        }
        else
            last_num = i;
    if (i != TLB_ENTRIES)
    {
        ++cpu_tlb.hit;
        phyaddr = cpu_tlb.entries[i].addr + tmpaddr.offset;
    }
    else
    {
        PDE pde = (PDE)hwaddr_read((cpu.cr3.page_directory_base << 12) + (tmpaddr.first * sizeof(PDE)), 4);
        Assert(pde.present, "Page dic not valid.");
        PTE pte = (PTE)hwaddr_read((pde.page_frame << 12) + (tmpaddr.second * sizeof(PTE)), 4);
        Assert(pte.present, "Page entry not valid.");
        phyaddr = (pte.page_frame << 12) + tmpaddr.offset;
        if (last_num < 0)
         {
            last_num = num;
            num = (num + 1) % TLB_ENTRIES;
         }
         cpu_tlb.entries[last_num].valid = 1;
         cpu_tlb.entries[last_num].tag = (addr & TLB_TAG_MASK) >> 12;
         cpu_tlb.entries[last_num].addr = pte.page_frame << 12;
    }
    return phyaddr;
}
