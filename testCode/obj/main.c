#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "ADS1256.h"
#include "stdio.h"
#include <unistd.h> // usleep関数を利用するため

#define SAMPLE_COUNT 10000  // データを保存するサンプル数
#define SAMPLING_PERIOD 100 // マイクロ秒単位でのサンプリング周期 (1秒 / 10kHz = 100μs)
#define TARGET_TIME 1 // 目標の取得時間（秒）

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
    timespec_get(&start_time, TIME_UTC); // 現在の時間を取得

    int data_index = 0;
    struct timespec sleep_time = {0, SAMPLING_PERIOD * 1000}; // サンプリング周期に合わせて待機時間を設定

    while ((current_time.tv_sec - start_time.tv_sec) < TARGET_TIME) // 目標の時間までデータを取得する
    {
        timespec_get(&current_time, TIME_UTC); // 現在の時間を取得

        // データを取得してバッファに保存
        data_buffer[data_index] = ADS1256_GetChannalValue(1) * 5.0 / 0x7fffff;

        // データ取得時の時間とデータを表示（ナノ秒単位）
        printf("%lld %f\n",
               (long long)((current_time.tv_sec - start_time.tv_sec) * 1000000000LL +
                           (current_time.tv_nsec - start_time.tv_nsec)),
               data_buffer[data_index]);

        data_index++;

        // サンプリング周期に合わせて待機時間を調整
        nanosleep(&sleep_time, NULL);
    }

    DEV_ModuleExit();
    return 0;
}
