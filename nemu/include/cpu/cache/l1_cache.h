#ifndef L1_CACHE_H_INCLUDED
#define L1_CACHE_H_INCLUDED

#include "common.h"
#include <string.h>

#define CACHE_SIZE 65536
#define CACHE_BLOCK_SIZE 64
#define CACHE_WAYS 8
#define CACHE_OFFSET 6
#define CACHE_NUM 7
#define CACHE_TAG (32 - CACHE_NUM - CACHE_OFFSET)
#define CACHE_MASK 0x3fL

#pragma pack(1)
typedef union _cache_addr_l1
{
    struct
    {
        uint32_t tag:CACHE_TAG;
        uint32_t num:CACHE_NUM;
        uint32_t off:CACHE_OFFSET;
    };
    uint32_t addr;
} cache_addr_l1;
#pragma pack(0)

typedef struct _cache_entry_l1
{
    char valid;
    uint64_t tag:CACHE_TAG;
    char buf[CACHE_BLOCK_SIZE];
} cache_entry_l1;

typedef struct _cache_l1
{
    cache_entry_l1 entries[(2 << CACHE_NUM) - 1][CACHE_WAYS];
    uint32_t instrs;
    uint32_t hit;
} cache_l1;

extern cache_l1 l1_cache;

uint32_t l1_cache_read(hwaddr_t addr, size_t len);

void l1_cache_write(hwaddr_t addr, size_t len, uint32_t data);

#endif // CACHE_H_INCLUDED
