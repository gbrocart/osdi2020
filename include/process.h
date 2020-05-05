#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "types.h"

#define MAX_TASK_QUEUE 64
#define STACK_SIZE 4096

typedef struct Task {
    unsigned long func;
    unsigned long stack;
} task;

uint64_t get_current();

void foo();

void bar();

void context_switch(uint64_t);

void privilege_task_create(unsigned long func);

#endif
