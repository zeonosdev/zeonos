// task.c - Otak Scheduler nya
#include "zeonos.h"

struct Task task_list[MAX_TASKS];
int current_task = -1;
int task_count = 0;

void task1() {
    int counter = 0;
    while(1) {
        print_at("TASK A JALAN ", counter % 60, 5, COLOR_GREEN);
        counter++;
        for(int i=0; i<1000000; i++); // delay
    }
}

void task2() {
    int counter = 0;
    while(1) {
        print_at("TASK B JALAN ", counter % 60, 7, COLOR_WHITE);
        counter++;
        for(int i=0; i<1000000; i++); // delay
    }
}

int create_task(void (*func)()) {
    if(task_count >= MAX_TASKS) return -1;

    struct Task* t = &task_list[task_count];
    t->id = task_count;
    t->state = 1;
    t->entry_point = func;
    t->stack_pointer = &t->stack[STACK_SIZE - 1];

    task_count++;
    return t->id;
}

void scheduler_tick() {
    // Ganti ke task berikutnya
    current_task = (current_task + 1) % task_count;
    print_at("SWITCH! ", 70, 0, COLOR_RED);
}

void init_scheduler() {
    create_task(task1);
    create_task(task2);
    current_task = 0;
}
