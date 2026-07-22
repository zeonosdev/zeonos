/*
 * ZeonOS - Process Management
 * process/process.c
 * 
 * Process/Task creation and management
 */

#include "../kernel.h"

/* Process structure */
typedef struct {
    uint32_t pid;
    char name[64];
    uint64_t entry_point;
    uint64_t stack_pointer;
    uint8_t state;  /* 0=ready, 1=running, 2=waiting, 3=terminated */
} process_t;

#define MAX_PROCESSES 256
static process_t processes[MAX_PROCESSES];
static int process_count = 0;
static uint32_t next_pid = 1;

/**
 * Create new process
 */
uint32_t process_create(const char* name, uint64_t entry_point) {
    if (process_count >= MAX_PROCESSES) {
        return 0;  /* Error: max processes reached */
    }
    
    process_t* proc = &processes[process_count];
    
    proc->pid = next_pid++;
    string_copy(proc->name, name);
    proc->entry_point = entry_point;
    proc->state = 0;  /* Ready */
    
    /* Allocate stack */
    proc->stack_pointer = (uint64_t)kernel_malloc(0x2000);  /* 8KB stack */
    
    process_count++;
    
    return proc->pid;
}

/**
 * Get process by PID
 */
process_t* process_get(uint32_t pid) {
    for (int i = 0; i < process_count; i++) {
        if (processes[i].pid == pid) {
            return &processes[i];
        }
    }
    return NULL;
}

/**
 * Terminate process
 */
void process_terminate(uint32_t pid) {
    process_t* proc = process_get(pid);
    if (proc) {
        proc->state = 3;  /* Terminated */
        kernel_free((void*)proc->stack_pointer);
    }
}
