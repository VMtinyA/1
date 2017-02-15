#include <stdio.h>
#include <time.h>
#include "sync_threads_func.h"

void SI2_interrupt(void);
unsigned long SI2_count = 0;

int main(int argc, char *argv[])
{

    unsigned short i;
    struct timespec tt;

    sync_threads_start();

    // инициализация портов ввода-вывода (RS-485 для работы с ТЛГ и RS-422 для работы на вывод наружу, ISA для обмена с МАЦП)

    // иницализация работы с сигналом прерывания СИ2
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


