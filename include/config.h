/*
 * ZeonOS Build Configuration
 * include/config.h
 */

#ifndef CONFIG_H
#define CONFIG_H

/* Kernel configuration */
#define KERNEL_VERSION "1.0.0-alpha"
#define KERNEL_AUTHOR "ZeonOS Development Team"

/* Memory configuration */
#define HEAP_SIZE 0x100000      /* 1MB heap */
#define MAX_MEMORY_BLOCKS 256
#define PAGE_SIZE 4096

/* Process configuration */
#define MAX_PROCESSES 256
#define STACK_SIZE 0x2000       /* 8KB per process */

/* Timer configuration */
#define PIT_FREQUENCY 1000      /* 1000 Hz */

/* Debug configuration */
#define DEBUG_MODE 1
#define DEBUG_SERIAL 1

/* Feature flags */
#define ENABLE_PAGING 1
#define ENABLE_INTERRUPTS 1
#define ENABLE_MULTITASKING 1

#endif /* CONFIG_H */
