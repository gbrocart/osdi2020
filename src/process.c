#include "types.h"
#include "process.h"
#include "printf.h"
#include "time.h"

uint8_t curr_pid = 0;
task *task_pool[MAX_TASK_QUEUE];
char kstack_pool[MAX_TASK_QUEUE][STACK_SIZE];


// returns the id of the current task
uint64_t get_current() {
    uint64_t t;
    asm volatile("mrs %0, tpidr_el1" : "=r" (t));
    return t;
}

void switch_to(task *prev, task *next) {
    asm volatile (
    "stp x19, x20, [x0, 16 * 0]  \t\n" // x19 stores functions, x20 function args
    "stp fp, lr, [x0, 16 * 1]    \t\n"
    "mov x9, sp                  \t\n"
    "str x9, [x0, 16 * 2]        \t\n"

    "ldp x19, x20, [x1, 16 * 0]  \t\n"
    "ldp fp, lr, [x1, 16 * 1]    \t\n"
    "ldr x9, [x1, 16 * 2]        \t\n"
    
    "mov sp,  x9                 \t\n"
    "msr tpidr_el1, x1           \t\n"
    );
}

void foo(){
  while(1) {
    printf("FOO\r\n");
    printf("Task id: %d\n", get_current());
    delay(1000);
    context_switch(1);
  }
}

void bar(){
  while(1) {
    printf("BAR\r\n");
    printf("Task id: %d\n", get_current());
    delay(1000);
    context_switch(0);
  }
}

void context_switch(uint64_t next){
    uint64_t prev = get_current();
    printf("%d %d\r\n", prev, next);
    switch_to(task_pool[prev], task_pool[next]);
    printf("stuck?\r\n");
    printf("%d", get_current());
}

void init_stack(char *stack, size_t size) {
    for (int i = 0; i < size; i++)
        *(stack + i) = 0;
}

void privilege_task_create(unsigned long func) {
    init_stack(kstack_pool[curr_pid], STACK_SIZE);
    task_pool[curr_pid]->func = func;
    task_pool[curr_pid]->stack = (unsigned long)(kstack_pool[curr_pid][0] + STACK_SIZE); 
    curr_pid++;
}
