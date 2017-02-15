#ifndef THREADS_FUNC_H
#define THREADS_FUNC_H

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h> // для отладки, потом отключить

#define Frequency0          1000
#define Frequency1          100
#define Frequency2          0.1

sem_t sem_SI2;
sem_t sem_0;
sem_t sem_1;
sem_t sem_2;

pthread_attr_t thread_attr;
pthread_attr_t thread_0_attr;
pthread_attr_t thread_1_attr;
pthread_attr_t thread_2_attr;

pthread_t thread_manager_id;
pthread_t thread_0_id;
pthread_t thread_1_id;
pthread_t thread_2_id;

struct sched_param thread_sched;
int thread_policy;

unsigned char thread_1_divider;

void sync_threads_start(void);
void threads_create(void);
void *thread_manager(void *param);
void *thread_0(void *param);
void *thread_1(void *param);
void *thread_2(void *param);

#endif // THREADS_FUNC_H
