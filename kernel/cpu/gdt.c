/*
 * ZeonOS - CPU Management
 * cpu/gdt.c
 * 
 * Global Descriptor Table implementation
 */

#include "../../kernel.h"

/* GDT Entry Structure */
typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t limit_high:4;
    uint8_t flags:4;
    uint8_t base_high;
} gdt_entry_t;

/* GDT Descriptor */
typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) gdt_descriptor_t;

#define GDT_ENTRIES 5
static gdt_entry_t gdt[GDT_ENTRIES];

/**
 * Set GDT entry
 */
static void set_gdt_entry(int index, uint32_t base, uint32_t limit,
                         uint8_t access, uint8_t flags) {
    gdt[index].limit_low = (limit & 0xFFFF);
    gdt[index].base_low = (base & 0xFFFF);
    gdt[index].base_mid = ((base >> 16) & 0xFF);
    gdt[index].access = access;
    gdt[index].limit_high = ((limit >> 16) & 0x0F);
    gdt[index].flags = (flags & 0x0F);
    gdt[index].base_high = ((base >> 24) & 0xFF);
}

/**
 * Initialize GDT
 */
void cpu_init_gdt(void) {
    kernel_println("  * Initializing GDT...");
    
    /* Null descriptor */
    set_gdt_entry(0, 0, 0, 0, 0);
    
    /* Kernel code segment */
    set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0x0F);
    
    /* Kernel data segment */
    set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0x0F);
    
    /* User code segment */
    set_gdt_entry(3, 0, 0xFFFFFFFF, 0xFA, 0x0F);
    
    /* User data segment */
    set_gdt_entry(4, 0, 0xFFFFFFFF, 0xF2, 0x0F);
    
    /* Load GDT */
    gdt_descriptor_t gdt_desc;
    gdt_desc.base = (uint64_t)gdt;
    gdt_desc.limit = (GDT_ENTRIES * sizeof(gdt_entry_t)) - 1;
    
    __asm__("lgdt %0" : : "m"(gdt_desc));
    
    kernel_println("    GDT initialized");
}
