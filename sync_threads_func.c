#include "sync_threads_func.h"

//*************************************************************************************************
// функция должна создать поток - диспетчер для управления синхронными потоками f0, f1, f2
// эта функция вводится для инкапсуляции всего, что принадлежит синхронным потокам, в данном модуле
// должна вызываться в main основной программы
void sync_threads_start(void) {

    // инициализация потока-диспетчера, работающего по сигналу прерывания СИ2
    // инициализация пускового семафора потока semSI2 - поток-диспетчер приостановлен
     sem_init(&sem_SI2, 1, 0);

     // задание атрибутов создаваемого потока
     ret = pthread_attr_init(&thread_manager_attr);
     if (ret != 0) {
         errno = ret;
         perror("Attribute creation failed");
     }
     // атрибут планирования -  планирование создаваемого потока,
     // явно заданное его атрибутами
     ret = pthread_attr_setinheritsched(&thread_manager_attr, PTHREAD_EXPLICIT_SCHED);
     if (ret != 0) {
         errno = ret;
         perror("Setting inheritsched failed");
     }
     // установка атрибута стратегии планирования (FIFO)
     ret = pthread_attr_setschedpolicy(&thread_manager_attr, SCHED_FIFO);
     if (ret != 0) {
         errno = ret;
         perror("Setting scheduling policy failed");
     }
     // установка значения приоритета
     max_priority = sched_get_priority_max(SCHED_FIFO); // 99
     min_priority = sched_get_priority_min(SCHED_FIFO); // 1
     thread_sched.sched_priority = max_priority;
     ret = pthread_attr_setschedparam(&thread_manager_attr, &thread_sched);
     if (ret != 0) {
         errno = ret;
         perror("Setting schedparam failed");
     }

     // создание потока с атрибутом thread_manager_attr
     ret = pthread_create(&thread_manager_id, &thread_manager_attr, &thread_manager, NULL);
     if (ret) {
         errno = ret;
         perror("thread_manager_create error");
     }

     // удаление использованных атрибутов
     pthread_attr_destroy(&thread_manager_attr);

 }
//****************************************************************************************************************
// вспомогательная функция для создания всех циклических синхронных потоков
// и инициализации всех их семафоров
// для вызова из функции-диспетчера
void threads_create(void) {

    // инициализация потока 0 с функциями обработки с частотой f0 = 1000 Гц
    // инициализация пускового семафора потока sem_0 - поток 0 приостановлен
    sem_init(&sem_0, 1, 0);

    // задание атрибутов создаваемого потока
    ret0 = pthread_attr_init(&thread_0_attr);
    if (ret0 != 0) {
        errno = ret0;
        perror("0: Attribute creation failed");
    }
    // атрибут планирования -  планирование создаваемого потока,
    // явно заданное его атрибутами
    ret0 = pthread_attr_setinheritsched(&thread_0_attr, PTHREAD_EXPLICIT_SCHED);
    if (ret0 != 0) {
        errno = ret0;
        perror("0: Setting inheritsched failed");
    }
    // установка атрибута стратегии планирования (FIFO)
    ret0 = pthread_attr_setschedpolicy(&thread_0_attr, SCHED_FIFO);
    if (ret0 != 0) {
        errno = ret0;
        perror("0: Setting scheduling policy failed");
    }
    // установка значения приоритета
    thread_sched.sched_priority = max_priority - 1;
    ret0 = pthread_attr_setschedparam(&thread_0_attr, &thread_sched);
    if (ret0 != 0) {
        errno = ret0;
        perror("0: Setting schedparam failed");
    }

    // создание потока с атрибутом thread_0_attr
    ret0 = pthread_create(&thread_0_id, &thread_0_attr, &thread_0, NULL);
    if (ret0) {
        errno = ret0;
        perror("thread_0_create error");
    }

    // удаление использованных атрибутов
    pthread_attr_destroy(&thread_0_attr);


    // инициализация потока 1 с функциями обработки с частотой f1 = 100/200 Гц (частоту задать через #define)
    // инициализация пускового семафора потока sem1 значением 0
    sem_init(&sem_1, 1, 0);
    pthread_attr_init(&thread_1_attr);
    ret = pthread_create(&thread_1_id, &thread_1_attr, &thread_1, NULL);
    if (ret) {
        errno = ret;
        perror("thread1_create error");
    }
    //pthread_getschedparam(thread_manager, &thread_policy, &thread_sched);
    //thread_policy = SCHED_FIFO;
    //thread_sched.sched_priority = 12;
    //pthread_setschedparam(thread_manager, thread_policy, &thread_sched);

    // инициализация потока 2 с функциями обработки с частотой f2 = 1/10 Гц (частоту задать через #define)
    // инициализация пускового семафора потока sem2 значением 0
    sem_init(&sem_2, 1, 0);

    // задание атрибутов создаваемого потока
    ret2 = pthread_attr_init(&thread_2_attr);
    if (ret2 != 0) {
        errno = ret2;
        perror("2: Attribute creation failed");
    }
    // атрибут планирования -  планирование создаваемого потока,
    // явно заданное его атрибутами
    ret2 = pthread_attr_setinheritsched(&thread_2_attr, PTHREAD_EXPLICIT_SCHED);
    if (ret2 != 0) {
        errno = ret2;
        perror("2: Setting inheritsched failed");
    }
    // установка атрибута стратегии планирования (FIFO)
    ret2 = pthread_attr_setschedpolicy(&thread_2_attr, SCHED_FIFO);
    if (ret2 != 0) {
        errno = ret2;
        perror("2: Setting scheduling policy failed");
    }
    // установка значения приоритета
    thread_sched.sched_priority = min_priority;
    ret2 = pthread_attr_setschedparam(&thread_2_attr, &thread_sched);
    if (ret2 != 0) {
        errno = ret2;
        perror("2: Setting schedparam failed");
    }

    // создание потока с атрибутом thread_2_attr
    ret2 = pthread_create(&thread_2_id, &thread_2_attr, &thread_2, NULL);
    if (ret2) {
        errno = ret2;
        perror("thread_2_create error");
    }

    // удаление использованных атрибутов
    pthread_attr_destroy(&thread_2_attr);

}

//****************************************************************************************************************
// функция-диспетчер циклических потоков
// при вызове должна создать все потоки и управлять ими
void *thread_manager(void *param) {

    threads_create();
    unsigned char thread_1_divider = 0;

    while (1) {

        printf("tm\n");
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
