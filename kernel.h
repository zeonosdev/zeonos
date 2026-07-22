/*
 * ZeonOS Kernel Header
 * kernel.h
 * 
 * Function declarations, type definitions, and macros
 * for the ZeonOS kernel
 */

#ifndef KERNEL_H
#define KERNEL_H

/* Standard integer types */
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long int64_t;

/* Memory management */
void kernel_init_memory(void);
void* kernel_malloc(uint64_t size);
void kernel_free(void* ptr);
void kernel_memzero(void* ptr, uint64_t size);
void kernel_memcpy(void* dest, const void* src, uint64_t size);

/* Interrupts */
void kernel_init_interrupts(void);

/* Timer */
void kernel_init_timer(void);
uint64_t kernel_get_uptime_ms(void);
uint64_t kernel_get_ticks(void);
void kernel_tick(void);
void kernel_sleep_ms(uint32_t ms);

/* Video/Console */
void kernel_clear_screen(void);
void kernel_putchar(char c);
void kernel_print(const char* str);
void kernel_println(const char* str);
void kernel_print_int(int num);

/* CPU operations */
uint64_t kernel_rdmsr(uint32_t msr);
void kernel_wrmsr(uint32_t msr, uint64_t value);
uint64_t kernel_read_cr0(void);
void kernel_write_cr0(uint64_t value);
uint64_t kernel_read_cr3(void);
void kernel_write_cr3(uint64_t value);

/* System control */
void kernel_halt_cpu(void);
void kernel_halt(void);
void kernel_panic(const char* message);
void kernel_main(void);

#endif /* KERNEL_H */
