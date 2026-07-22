/*
 * ZeonOS - Memory Management
 * kernel/memory/paging.c
 * 
 * Virtual memory and paging implementation
 */

#include "../../kernel.h"

/* Page table entry structure */
typedef struct {
    uint64_t present:1;
    uint64_t writable:1;
    uint64_t user:1;
    uint64_t write_through:1;
    uint64_t cache_disabled:1;
    uint64_t accessed:1;
    uint64_t dirty:1;
    uint64_t huge_page:1;
    uint64_t global:1;
    uint64_t reserved:3;
    uint64_t address:40;
    uint64_t reserved2:12;
} page_entry_t;

#define PAGE_SIZE 4096
#define PAGE_ENTRIES 512

static page_entry_t pml4[PAGE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));
static page_entry_t pdp[PAGE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));
static page_entry_t pd[PAGE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));
static page_entry_t pt[PAGE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));

/**
 * Initialize paging structures
 */
void mem_init_paging(void) {
    kernel_println("  * Initializing paging...");
    
    /* Zero out page tables */
    kernel_memzero(pml4, sizeof(pml4));
    kernel_memzero(pdp, sizeof(pdp));
    kernel_memzero(pd, sizeof(pd));
    kernel_memzero(pt, sizeof(pt));
    
    /* Map first 2MB with identity mapping */
    for (int i = 0; i < 512; i++) {
        page_entry_t entry;
        entry.present = 1;
        entry.writable = 1;
        entry.address = (i * PAGE_SIZE) >> 12;
        pt[i] = entry;
    }
    
    /* Set up page table hierarchy */
    page_entry_t pd_entry, pdp_entry, pml4_entry;
    
    pd_entry.present = 1;
    pd_entry.writable = 1;
    pd_entry.address = ((uint64_t)pt) >> 12;
    pd[0] = pd_entry;
    
    pdp_entry.present = 1;
    pdp_entry.writable = 1;
    pdp_entry.address = ((uint64_t)pd) >> 12;
    pdp[0] = pdp_entry;
    
    pml4_entry.present = 1;
    pml4_entry.writable = 1;
    pml4_entry.address = ((uint64_t)pdp) >> 12;
    pml4[0] = pml4_entry;
    
    /* Load CR3 with PML4 address */
    kernel_write_cr3((uint64_t)pml4);
    
    kernel_println("    Paging initialized");
}

/**
 * Map virtual address to physical address
 */
void mem_map_page(uint64_t vaddr, uint64_t paddr) {
    uint64_t pml4_index = (vaddr >> 39) & 0x1FF;
    uint64_t pdp_index = (vaddr >> 30) & 0x1FF;
    uint64_t pd_index = (vaddr >> 21) & 0x1FF;
    uint64_t pt_index = (vaddr >> 12) & 0x1FF;
    
    /* Create entries if needed */
    page_entry_t entry;
    entry.present = 1;
    entry.writable = 1;
    entry.address = (paddr >> 12);
    
    pt[pt_index] = entry;
    
    /* Flush TLB */
    __asm__("invlpg (%0)" : : "r"(vaddr));
}
