// zeonos.h
#ifndef ZEONOS_H
#define ZEONOS_H

#define MAX_TASKS 10
#define STACK_SIZE 4096

typedef unsigned int uint32_t;

// Warna buat VGA
enum vga_color {
    COLOR_BLACK = 0,
    COLOR_GREEN = 2,
    COLOR_WHITE = 15,
    COLOR_RED = 12
};

// Struktur 1 Task
struct Task {
    int id;
    int state; // 0 = mati, 1 = jalan
    uint32_t* stack_pointer;
    uint32_t stack[STACK_SIZE];
    void (*entry_point)();
};

// Global
extern struct Task task_list[MAX_TASKS];
extern int current_task;
extern int task_count;

// Fungsi dasar
void clear_screen();
void print_at(const char* str, int x, int y, uint32_t color);
void init_scheduler();
int create_task(void (*func)());
void scheduler_tick();
void switch_task();

#endif
