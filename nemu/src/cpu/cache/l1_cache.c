#include "common.h"
#include "cpu/cache/l1_cache.h"
#include "misc.h"
#define HW_MEM_SIZE (1<<27)

cache_l1 l1_cache;
uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

void l1_cache_get(cache_addr_l1 addr, uint8_t *temp)
{
    int i, last_num = -1;
    addr.addr &= ~CACHE_MASK;
    ++l1_cache.instrs;
    for (i=0; i<CACHE_WAYS; ++i)
        if (l1_cache.entries[addr.num][i].valid)
        {
            if (l1_cache.entries[addr.num][i].tag == addr.tag)
                break;
        }
        else
            last_num = i;
    if (i != CACHE_WAYS)
    {
        memcpy(temp, l1_cache.entries[addr.num][i].buf, CACHE_BLOCK_SIZE);
        ++l1_cache.hit;
    }
    else
    {
        uint32_t t[CACHE_BLOCK_SIZE/4];
        for (i=0; i<CACHE_BLOCK_SIZE/4; ++i)
            t[i] = dram_read(addr.addr + (i << 2), 4);

        if (last_num < 0)
            last_num = 0;

        l1_cache.entries[addr.num][last_num].valid = 1;
        l1_cache.entries[addr.num][last_num].tag = addr.tag;
        memcpy(l1_cache.entries[addr.num][last_num].buf, t, CACHE_BLOCK_SIZE);
        memcpy(temp, t, CACHE_BLOCK_SIZE);
    }
    //printf("%u memory instr, %u hit\n", l1_cache.instrs, l1_cache.hit);
}

void l1_cache_put(cache_addr_l1 addr, void *data, uint8_t *mask)
{
    int i;
    addr.addr &= ~CACHE_MASK;
    for (i=0; i<CACHE_WAYS; ++i)
        if ((l1_cache.entries[addr.num][i].valid) && (l1_cache.entries[addr.num][i].tag == addr.tag))
            break;
    if (i != CACHE_WAYS)
    {
        memcpy_with_mask(l1_cache.entries[addr.num][i].buf, data, CACHE_BLOCK_SIZE, mask);
    }
}

uint32_t l1_cache_read(hwaddr_t addr, size_t len)
{
    uint32_t offset = addr & CACHE_MASK;
    uint8_t temp[2 * CACHE_BLOCK_SIZE];
    cache_addr_l1 l1_addr;
    l1_addr.addr = addr;

    l1_cache_get(l1_addr, temp);

    if(offset + len > CACHE_BLOCK_SIZE)
    {
        /* data cross the burst boundary */
        l1_addr.addr += CACHE_BLOCK_SIZE;
        l1_cache_get(l1_addr, temp + CACHE_BLOCK_SIZE);
    }

    return unalign_rw(temp + offset, 4);
}

void l1_cache_write(hwaddr_t addr, size_t len, uint32_t data)
{
    uint32_t offset = addr & CACHE_MASK;
    uint8_t temp[2 * CACHE_BLOCK_SIZE];
    uint8_t mask[2 * CACHE_BLOCK_SIZE];
    cache_addr_l1 l1_addr;
    l1_addr.addr = addr;

    dram_write(addr, len, data);

    memset(mask, 0, CACHE_BLOCK_SIZE * 2);

    *(uint32_t *)(temp + offset) = data;
    memset(mask + offset, 1, len);

    l1_cache_put(l1_addr, temp, mask);

    if(offset + len > CACHE_BLOCK_SIZE)
    {
        /* data cross the burst boundary */
        l1_addr.addr += CACHE_BLOCK_SIZE;
        l1_cache_put(l1_addr, temp + CACHE_BLOCK_SIZE, mask + CACHE_BLOCK_SIZE);
    }
}
