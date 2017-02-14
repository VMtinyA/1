#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define Frequency0          1000
#define Frequency1          100

void SI2_interrupt(void);
void *thread_manager(void *param);
void *thread_0(void *param);
void *thread_1(void *param);

unsigned long SI2_count = 0;

sem_t sem_SI2;
sem_t sem_0;
sem_t sem_1;

unsigned char thread_1_divider = 0;

int main(int argc, char *argv[])
{

    pthread_attr_t thread_attr;
    pthread_attr_t thread_0_attr;
    pthread_attr_t thread_1_attr;

    pthread_t thread_manager_id;
    pthread_t thread_0_id;
    pthread_t thread_1_id;

    struct sched_param thread_sched;
    int thread_policy;

    unsigned short i;
    struct timespec tt;

    // инициализация портов ввода-вывода (RS-485 для работы с ТЛГ и RS-422 для работы на вывод наружу, ISA для обмена с МАЦП)

    // инициализация потока-диспетчера, работающего по сигналу прерывания СИ2
    // инициализация пускового семафора потока semSI2 - поток-диспетчер приостановлен
    sem_init(&sem_SI2, 1, 0);
    pthread_attr_init(&thread_attr);
    pthread_create(&thread_manager_id, &thread_attr, &thread_manager, NULL);
    //pthread_getschedparam(thread_manager, &thread_policy, &thread_sched);
    //thread_policy = SCHED_FIFO;
    //thread_sched.sched_priority = 10;
    //pthread_setschedparam(thread_manager, thread_policy, &thread_sched);

    // иницализация работы с сигналом прерывания СИ2


    // инициализация потока 0 с функциями обработки с частотой f0 = 1000 Гц
    // инициализация пускового семафора потока sem_0 - поток 0 приостановлен
    sem_init(&sem_0, 1, 0);
    pthread_create(&thread_0_id, &thread_attr, &thread_0, NULL);
    //pthread_getschedparam(thread_manager, &thread_policy, &thread_sched);
    //thread_policy = SCHED_FIFO;
    //thread_sched.sched_priority = 11;
    //pthread_setschedparam(thread_manager, thread_policy, &thread_sched);

    // инициализация потока 1 с функциями обработки с частотой f1 = 100/200 Гц (частоту задать через #define)
    // инициализация пускового семафора потока sem1 значением 0
    sem_init(&sem_1, 1, 0);
    pthread_create(&thread_1_id, &thread_attr, &thread_1, NULL);
    //pthread_getschedparam(thread_manager, &thread_policy, &thread_sched);
    //thread_policy = SCHED_FIFO;
    //thread_sched.sched_priority = 12;
    //pthread_setschedparam(thread_manager, thread_policy, &thread_sched);

    // имитация прерывания с частотой СИ2
    tt.tv_sec = 1;
    tt.tv_nsec = 0;
    nanosleep(&tt, NULL);
    //tt.tv_sec = 0;
    //tt.tv_nsec = 100000000;
    for (i = 0; i < 100; ++i) {
        SI2_interrupt();
        nanosleep(&tt, NULL);
    }

    //
    //pthread_join(thread_manager_id, NULL);

    printf("finish\n");

    return 0;
}

//****************************************************************************************************************
// обработчик прерывания СИ2
void SI2_interrupt(void) {

    // проверка состояния

    SI2_count++;
    // освобождение семафора semSI2
    printf("SI2 %lu\n", SI2_count);
    sem_post(&sem_SI2);

}

//****************************************************************************************************************
// функция-диспетчер циклических потоков
void *thread_manager(void *param) {

    while (1) {

        // попытка захвата семафора semSI2
        sem_wait(&sem_SI2);

        printf("tm\n");

        // запуск семафора потока 0
        sem_post(&sem_0);

        if (thread_1_divider) {
            --thread_1_divider;
        } else {
            thread_1_divider = 4;
            sem_post(&sem_1);
        }

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
