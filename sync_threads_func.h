#ifndef THREADS_FUNC_H
#define THREADS_FUNC_H

#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <sched.h>
#include <stdio.h> // для отладки, потом отключить

#define Frequency0          1000
#define Frequency1          100
#define Frequency2          0.1

sem_t sem_SI2;
sem_t sem_0;
sem_t sem_1;
sem_t sem_2;

pthread_attr_t thread_manager_attr;
pthread_attr_t thread_0_attr;
pthread_attr_t thread_1_attr;
pthread_attr_t thread_2_attr;

pthread_t thread_manager_id;
pthread_t thread_0_id;
pthread_t thread_1_id;
pthread_t thread_2_id;

struct sched_param thread_sched; // для задания приоритетов
int max_priority;
int min_priority;

int ret, ret0, ret1, ret2; // для контроля создания потоков

// функция должна создать поток - диспетчер для управления синхронными потоками f0, f1, f2
// эта функция вводится для "инкапсуляции" всего, что принадлежит синхронным потокам, в данном модуле
// должна вызываться в main основной программы
void sync_threads_start(void);

// вспомогательная функция для создания всех циклических синхронных потоков
// и инициализации всех их семафоров
// для вызова из функции-диспетчера
void threads_create(void);

// функция-диспетчер циклических потоков
// при вызове должна создать все потоки и управлять ими
void *thread_manager(void *param);

// функции потоков
void *thread_0(void *param);
void *thread_1(void *param);
void *thread_2(void *param);

#endif // THREADS_FUNC_H
