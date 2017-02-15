#include "sync_threads_func.h"

//*************************************************************************************************
// функция должна создать поток - диспетчер для управления синхронными потоками f0, f1, f2
// эта функция вводится для инкапсуляции всего, что принадлежит синхронным потокам, в данном модуле
// должна вызываться в main основной программы
void sync_threads_start(void) {

    // инициализация потока-диспетчера, работающего по сигналу прерывания СИ2
    // инициализация пускового семафора потока semSI2 - поток-диспетчер приостановлен
     sem_init(&sem_SI2, 1, 0);
     pthread_attr_init(&thread_attr);
     pthread_create(&thread_manager_id, &thread_attr, &thread_manager, NULL);
     //pthread_getschedparam(thread_manager, &thread_policy, &thread_sched);
     //thread_policy = SCHED_FIFO;
     //thread_sched.sched_priority = 10;
     //pthread_setschedparam(thread_manager, thread_policy, &thread_sched);
}

//****************************************************************************************************************
// вспомогательная функция для создания всех циклических синхронных потоков
// и инициализации всех их семафоров
// для вызова из функции-диспетчера
void threads_create(void) {

    // инициализация потока 0 с функциями обработки с частотой f0 = 1000 Гц
    // инициализация пускового семафора потока sem_0 - поток 0 приостановлен
    sem_init(&sem_0, 1, 0);
    pthread_attr_init(&thread_0_attr);
    pthread_create(&thread_0_id, &thread_0_attr, &thread_0, NULL);
    //pthread_getschedparam(thread_manager, &thread_policy, &thread_sched);
    //thread_policy = SCHED_FIFO;
    //thread_sched.sched_priority = 11;
    //pthread_setschedparam(thread_manager, thread_policy, &thread_sched);

    // инициализация потока 1 с функциями обработки с частотой f1 = 100/200 Гц (частоту задать через #define)
    // инициализация пускового семафора потока sem1 значением 0
    sem_init(&sem_1, 1, 0);
    pthread_attr_init(&thread_1_attr);
    pthread_create(&thread_1_id, &thread_1_attr, &thread_1, NULL);
    //pthread_getschedparam(thread_manager, &thread_policy, &thread_sched);
    //thread_policy = SCHED_FIFO;
    //thread_sched.sched_priority = 12;
    //pthread_setschedparam(thread_manager, thread_policy, &thread_sched);

    // инициализация потока 2 с функциями обработки с частотой f2 = 1/10 Гц (частоту задать через #define)
    // инициализация пускового семафора потока sem2 значением 0
    sem_init(&sem_2, 1, 0);
    pthread_attr_init(&thread_2_attr);
    pthread_create(&thread_2_id, &thread_2_attr, &thread_2, NULL);
    //pthread_getschedparam(thread_manager, &thread_policy, &thread_sched);
    //thread_policy = SCHED_FIFO;
    //thread_sched.sched_priority = 12;
    //pthread_setschedparam(thread_manager, thread_policy, &thread_sched);

}

//****************************************************************************************************************
// функция-диспетчер циклических потоков
// при вызове должна создать все потоки и управлять ими
void *thread_manager(void *param) {

    threads_create();
    unsigned char thread_1_divider = 0;

    while (1) {

        // попытка захвата семафора semSI2 (должен быть выдан обработчиком прерывания SI2)
        sem_wait(&sem_SI2);

        // управление потоками
        sem_post(&sem_0);

        if (thread_1_divider) {
            --thread_1_divider;
        } else {
            thread_1_divider = 9;
            sem_post(&sem_1);
        }

        sem_post(&sem_2);

    }
}

//****************************************************************************************************************
// функция потока 0 (1000 Гц)
void *thread_0(void *param) {

    while (1) {

        // попытка захвата семафора sem_0
        sem_wait(&sem_0);

        // какие-то действия
        printf("t0\n");

    }

}

//****************************************************************************************************************
// функция потока 1 (100/200 Гц)
void *thread_1(void *param) {

    while (1) {

        // попытка захвата семафора sem_1
        sem_wait(&sem_1);

        // какие-то действия
        printf("t1\n");

    }

}

//****************************************************************************************************************
// функция потока 2 (1/10 Гц)
void *thread_2(void *param) {

    while (1) {

        // попытка захвата семафора sem_2
        sem_wait(&sem_2);

        // какие-то действия
        printf("t2\n");

    }

}
