#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "ADS1256.h"
#include "stdio.h"
#include <time.h>
#include <string.h>
#include <sys/timeb.h>
#include <unistd.h> // usleep関数を利用するため

#define SAMPLE_COUNT 500  // データを保存するサンプル数

int main(void)
{
    UDOUBLE ADC[8], i;
    printf("demo\r\n");
    DEV_ModuleInit();

    if (ADS1256_init() == 1)
    {
        printf("\r\nEND                  \r\n");
        DEV_ModuleExit();
        exit(0);
    }

    float data_buffer[SAMPLE_COUNT]; // データを一時的に保存するバッファ
    struct timespec start_time, current_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    clock_gettime(CLOCK_MONOTONIC, &current_time);

    int data_index = 0;
    struct timespec sleep_time = {0, 1000}; // 1μ秒ごとの待機時間を設定（nanosleep()関数はナノ秒単位で待機）

    while (data_index < SAMPLE_COUNT) // サンプル数だけデータを取得する
    {
        // データを取得してバッファに保存
        data_buffer[data_index] = ADS1256_GetChannalValue(1) * 5.0 / 0x7fffff;
        data_index++;

        // 待機時間を使って1μ秒待つ
        nanosleep(&sleep_time, NULL);
    }

    // データと時間を一気に出力
    printf("Data\n");
    for (int i = 0; i < data_index; i++)
    {
        printf("%f\n", data_buffer[i]);
    }

    DEV_ModuleExit();
    return 0;
}
