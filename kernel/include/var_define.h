#ifndef __VAR_DEFIEN__
#define __VAR_DEFINE__

#include "list.h"
#include "../../cpu/include/hw_include.h"
#include "../../libc/include/printf.h"
#define SYSTEM_BYTE 32

/*for timer function*/
#define SOFTWARE_TIMER 0
#define CYCLE 1

/*for delay*/
#define DELAY 2
void os_delay(U32 timeslice);

typedef  void (*TASK_ENTRY)(void *p_arg);
typedef  void (*FUNC_PTR)(void *p_arg);

#define STACK unsigned int 

/*for task control block*/
typedef struct TCB_STR{
    STACK *stack_ptr;
    LIST   list;
    U8     prio;
    BOOL   state; // Only two state, run or forbit run
}TCB;

/*time manage*/
typedef struct TICK_STR{
    LIST list;
    U8   style; // timer,delay,wait signer
    U32  timeout;
    TCB  *tcb;
    FUNC_PTR func; // for software timer
    void *func_arg;
    BOOL  period;
    U32  timeout_copy;
}TICK;

/*for semaphore*/
typedef struct SEM_STR{
    U32 count;
    U8  *name;
    TCB *tcb;
}SEM;

void sem_init(SEM *semaphore, U32 num);
void sem_put(SEM *semaphore);
void sem_get(SEM *semaphore);


extern TCB task_prio_queue[SYSTEM_BYTE];
extern TCB *new_task;
extern TCB *old_task;
extern U32 task_prio_map;

extern TCB idle_tcb;
extern U32 idle_stack[4*100];

#define bit_clear(num, i) num = num & (~(1<<i))
#define bit_set(num, i)   num = num | (1<<i)

TCB* bit_first_one_search(U32 num);
void prio_ready_queue_init();
void prio_ready_queue_insert(TCB *tcb);
void schedule();
void prio_ready_queue_delete(TCB *tcb);
void prio_ready_queue_insert_head(TCB *tcb);


void tick_queue_init();
void start_which_task(TCB *tcb);

void idle_task(void *arg);

#endif