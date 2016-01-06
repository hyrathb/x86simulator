#include "common.h"
#include "cpu/cache/l2_cache.h"
#include "misc.h"
#define HW_MEM_SIZE (1<<27)

cache_l2 l2_cache;
uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

void l2_write_back(int num, int i)
{
    if (l2_cache.entries[num][i].valid && l2_cache.entries[num][i].dirty)
    {
        int addrbase = (l2_cache.entries[num][i].tag << (CACHE_L2_OFFSET + CACHE_L2_NUM)) | (num << CACHE_L2_OFFSET);
        int j;
        for (j=0; j<CACHE_L2_BLOCK_SIZE; j+= 4)
        {
            dram_write(addrbase + j, 4, *(uint32_t *)(l2_cache.entries[num][i].buf + j));
        }
    }
}

int l2_cache_get(cache_addr_l2 addr, uint8_t *temp)
{
    static int rand = 0;
    int i, last_num = -1;
    addr.addr &= ~CACHE_L2_MASK;
    for (i=0; i<CACHE_L2_WAYS; ++i)
        if (l2_cache.entries[addr.num][i].valid)
        {
            if (l2_cache.entries[addr.num][i].tag == addr.tag)
                break;
        }
        else
            last_num = i;
    if (i != CACHE_L2_WAYS)
    {
        memcpy(temp, l2_cache.entries[addr.num][i].buf, CACHE_L2_BLOCK_SIZE);
        ++l2_cache.hit;
        return i;
    }
    else
    {
        uint32_t t[CACHE_L2_BLOCK_SIZE/4];
        for (i=0; i<CACHE_L2_BLOCK_SIZE/4; ++i)
            t[i] = dram_read(addr.addr + (i << 2), 4);

        if (last_num  < 0)
        {
            last_num = rand;
            rand = (rand + 1) % (CACHE_L2_WAYS);
        }

        l2_write_back(addr.num, last_num);
        l2_cache.entries[addr.num][last_num].valid = 1;
        l2_cache.entries[addr.num][last_num].dirty = 0;
        l2_cache.entries[addr.num][last_num].tag = addr.tag;
        memcpy(l2_cache.entries[addr.num][last_num].buf, t, CACHE_L2_BLOCK_SIZE);
        if (temp)
            memcpy(temp, t, CACHE_L2_BLOCK_SIZE);
        return last_num;
    }
    //printf("%u memory instr, %u hit\n", l2_cache.instrs, l2_cache.hit);
}

void l2_cache_put(cache_addr_l2 addr, void *data, uint8_t *mask, int dirty)
{
    int i;
    addr.addr &= ~CACHE_L2_MASK;
    for (i=0; i<CACHE_L2_WAYS; ++i)
        if (l2_cache.entries[addr.num][i].valid)
            if (l2_cache.entries[addr.num][i].tag == addr.tag)
                break;
    if (i == CACHE_L2_WAYS)
        i = l2_cache_get(addr, NULL);

    memcpy_with_mask(l2_cache.entries[addr.num][i].buf, data, CACHE_L2_BLOCK_SIZE, mask);
    l2_cache.entries[addr.num][i].dirty = dirty;
}

uint32_t l2_cache_read(hwaddr_t addr, size_t len)
{
    uint32_t offset = addr & CACHE_L2_MASK;
    uint8_t temp[2 * CACHE_L2_BLOCK_SIZE];
    cache_addr_l2 l2_addr;
    l2_addr.addr = addr;

    l2_cache_get(l2_addr, temp);

    if(offset + len > CACHE_L2_BLOCK_SIZE)
    {
        /* data cross the burst boundary */
        l2_addr.addr += CACHE_L2_BLOCK_SIZE;
        l2_cache_get(l2_addr, temp + CACHE_L2_BLOCK_SIZE);
    }

    return unalign_rw(temp + offset, 4);
}

void l2_cache_write(hwaddr_t addr, size_t len, uint32_t data)
{
    uint32_t offset = addr & CACHE_L2_MASK;
    uint8_t temp[2 * CACHE_L2_BLOCK_SIZE];
    uint8_t mask[2 * CACHE_L2_BLOCK_SIZE];
    cache_addr_l2 l2_addr;
    l2_addr.addr = addr;

    memset(mask, 0, CACHE_L2_BLOCK_SIZE * 2);

    *(uint32_t *)(temp + offset) = data;
    memset(mask + offset, 1, len);

    l2_cache_put(l2_addr, temp, mask, 1);

    if(offset + len > CACHE_L2_BLOCK_SIZE)
    {
        /* data cross the burst boundary */
        l2_addr.addr += CACHE_L2_BLOCK_SIZE;
        l2_cache_put(l2_addr, temp + CACHE_L2_BLOCK_SIZE, mask + CACHE_L2_BLOCK_SIZE, 1);
    }
}
