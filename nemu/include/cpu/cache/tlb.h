#ifndef TLB_H_INCLUDED
#define TLB_H_INCLUDED

#define TLB_ENTRIES 64
#define TLB_TAG_MASK 0xfffff000

#include "common.h"
#include "cpu/mmu.h"

typedef struct tlbentry_
{
    uint8_t valid;
    uint32_t tag;
    uint32_t addr;
} tlbentry;

typedef struct tlb_
{
    tlbentry entries[TLB_ENTRIES];
    uint32_t hit;
    uint32_t req;
} tlb;

extern tlb cpu_tlb;

typedef union
{
    lnaddr_t addr;
    struct
    {
        uint32_t offset:12;
        uint32_t second:10;
        uint32_t first:10;
    };
} tlbaddr;

uint32_t tlb_lookup(uint32_t addr);

#endif // TLB_H_INCLUDED
