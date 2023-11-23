#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "ADS1256.h"
#include "stdio.h"
#include <time.h>
#include <string.h>
#include <sys/timeb.h>

void Handler(int signo)
{
    //System Exit
    printf("\r\nEND                  \r\n");
    DEV_ModuleExit();

    exit(0);
}

int main(void)
{
    UDOUBLE ADC[8], i;
    printf("demo\r\n");
    DEV_ModuleInit();

    // Exception handling: ctrl + c
    signal(SIGINT, Handler);

    if (ADS1256_init() == 1)
    {
        printf("\r\nEND                  \r\n");
        DEV_ModuleExit();
        exit(0);
    }

    struct timespec delay_time;
    delay_time.tv_sec = 0;     // 10マイクロ秒 = 0秒
    delay_time.tv_nsec = 10000; // ナノ秒単位で10マイクロ秒 = 10000ナノ秒

    struct timespec start_time, current_time, elapsed_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    clock_gettime(CLOCK_MONOTONIC, &current_time);

    while ((current_time.tv_sec - start_time.tv_sec) < 5) // 5秒間ループする
    {
        clock_gettime(CLOCK_MONOTONIC, &elapsed_time);
        printf("Time: %ld seconds, %ld nanoseconds | Value: %f\n",
               elapsed_time.tv_sec - start_time.tv_sec,
               elapsed_time.tv_nsec - start_time.tv_nsec,
               ADS1256_GetChannalValue(1) * 5.0 / 0x7fffff);

        nanosleep(&delay_time, NULL); // 10マイクロ秒待つ

        clock_gettime(CLOCK_MONOTONIC, &current_time);
    }

    return 0;
}
