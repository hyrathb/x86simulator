#ifndef L2_CACHE_L2_H_INCLUDED
#define L2_CACHE_L2_H_INCLUDED

#include "common.h"
#include <string.h>

#define CACHE_L2_SIZE 4194304
#define CACHE_L2_BLOCK_SIZE 64
#define CACHE_L2_WAYS 16
#define CACHE_L2_OFFSET 6
#define CACHE_L2_NUM 12
#define CACHE_L2_TAG (32 - CACHE_L2_NUM - CACHE_L2_OFFSET)
#define CACHE_L2_MASK 0x3fL

#pragma pack(1)
typedef union _cache_addr_l2
{
    struct
    {
        uint32_t tag:CACHE_L2_TAG;
        uint32_t num:CACHE_L2_NUM;
        uint32_t off:CACHE_L2_OFFSET;
    };
    uint32_t addr;
} cache_addr_l2;
#pragma pack(0)

typedef struct _cache_entry_l2
{
    char valid;
    char dirty;
    uint64_t tag:CACHE_L2_TAG;
    char buf[CACHE_L2_BLOCK_SIZE];
} cache_entry_l2;

typedef struct _cache_l2
{
    cache_entry_l2 entries[(2 << CACHE_L2_NUM) - 1][CACHE_L2_WAYS];
    uint32_t hit;
} cache_l2;

extern cache_l2 l2_cache;

int l2_cache_get(cache_addr_l2 addr, uint8_t *temp);

void l2_cache_put(cache_addr_l2 addr, void *data, uint8_t *mask, int dirty);

uint32_t l2_cache_read(hwaddr_t addr, size_t len);

void l2_cache_write(hwaddr_t addr, size_t len, uint32_t data);

#endif // CACHE_L2_H_INCLUDED
