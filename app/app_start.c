#include <var_define.h>

#define msg_queue_test        0
#define sem_test              0
#define tick_tes              0
#define mutex_test            0
#define task_prio_change_test 0 

STACK stack1[4*100];
STACK stack2[4*100];
STACK stack3[4*100];
STACK stack4[4*100];
TCB tcb1;
TCB tcb2;
TCB tcb3;
TCB tcb4;

#if sem_test 
SEM sem;
#endif

#if mutex_test 
static int mutex_resource;
MUTEX mutex;
#endif

#if msg_queue_test
QUEUE my_queue;
MSG msg1;
MSG msg2;
MSG msg3;
U8 buffer[10];
#endif

void task1(void *arg)
{
    while (1){

#if msg_queue_test
        msg_put(&my_queue, &msg1, FIFO); 
        msg_put(&my_queue, &msg2, FIFO); 
        msg_put(&my_queue, &msg3, FIFO);
#endif

#if sem_test
        sem_put(&sem);
        os_printf("task1 = %d\n", sem.count );
#endif

#if mutex_test
        mut_get(&mutex);
        mutex_resource++;
        os_printf("task1 mutex_resource:%d\n", mutex_resource );
        mut_put(&mutex);
#endif 
        os_printf("task1 running\n");

#if task_prio_change_test
        task_prio_change(&tcb1, 5);
#endif
        os_delay(3);
    }
}

void task2(void *arg)
{
    while (1)
    {
#if msg_queue_test
        memset(buffer, 0, 10);
        msg_get(&my_queue, buffer);
        os_printf("first read = %s\n", buffer);
#endif

#if sem_test
        sem_get(&sem);
        os_printf("task2: %d\n", sem.count);
#endif
        os_printf("task2 running\n" );
        os_delay(3);
    };
}

void task3(void *arg)
{
    while (1)
    {
        os_printf("task3 runing\n");
        os_delay(10);
    }
}

void task4(void *arg)
{
    while (1)
    {
        os_printf("task4 running\n");
        os_delay(10);
    }
}

void app_main()
{
    task_create(&tcb1, (U8 *)"task1", task1, stack1, 3, 1);
    task_create(&tcb2, (U8 *)"task2", task2, stack2, 3, 1);
    task_create(&tcb3, (U8 *)"task3", task3, stack3, 3, 1);
    task_create(&tcb4, (U8 *)"task4", task4, stack4, 1, 1);

#if sem_test
    sem_init(&sem, "sem1", 1);
#endif 

#if mutex_test
    mut_init(&mutex, (U8 *)"mutex1");
#endif 

#if tick_test
    extern void test_tick();
    test_tick();
#endif

#if msg_queue_test
    msg1.buff = "abcd";
    msg2.buff = "abce";
    msg3.buff = "abcf";
#endif

#if task_prio_change_test
    task_prio_change(&tcb1, 1);
#endif 

}