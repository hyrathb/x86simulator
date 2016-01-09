#include "common.h"
#include "cpu/reg.h"
#include "cpu/cache/l1_cache.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
uint32_t seg_translate(uint32_t addr, uint16_t len ,uint8_t current_sreg);
uint32_t page_translate(lnaddr_t addr);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	//return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	return l1_cache_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	//dram_write(addr, len, data);
	return l1_cache_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
    Assert((addr & 4095) + len <= 4096, "WTF");
    hwaddr_t hwaddr = (cpu.cr0.pe &&  cpu.cr0.pg)?page_translate(addr):addr;
	return hwaddr_read(hwaddr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
    Assert((addr & 4095) + len <= 4096, "WTF");
    hwaddr_t hwaddr = (cpu.cr0.pe && cpu.cr0.pg)?page_translate(addr):addr;
	hwaddr_write(hwaddr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
    lnaddr_t lnaddr = (cpu.cr0.pe)?seg_translate(addr, len, cpu.current_sreg):addr;
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
    lnaddr_t lnaddr = (cpu.cr0.pe)?seg_translate(addr, len, cpu.current_sreg):addr;
	lnaddr_write(lnaddr, len, data);
}

