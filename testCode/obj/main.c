#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "ADS1256.h"
#include "stdio.h"
#include <unistd.h> // usleep関数を利用するため
#include <string.h>

#define SAMPLE_COUNT 15000  // データを保存するサンプル数
#define SAMPLING_PERIOD 100 // マイクロ秒単位でのサンプリング周期 (1秒 / 10kHz = 100μs)
#define TARGET_TIME 2// 目標の取得時間（秒）

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
    long long time_buffer[SAMPLE_COUNT]; // 時間経過を一時的に保存するバッファ
    struct timespec start_time, current_time;
    timespec_get(&start_time, TIME_UTC); // 現在の時間を取得

    int data_index = 0;
    struct timespec sleep_time = {0, SAMPLING_PERIOD * 1000}; // サンプリング周期に合わせて待機時間を設定

    while ((current_time.tv_sec - start_time.tv_sec) < TARGET_TIME) // 目標の時間までデータを取得する
    {
        timespec_get(&current_time, TIME_UTC); // 現在の時間を取得

        // データを取得してバッファに保存
        data_buffer[data_index] = ADS1256_GetChannalValue(1) * 5.0 / 0x7fffff;

        // 時間経過を保存
        time_buffer[data_index] = (current_time.tv_sec - start_time.tv_sec) * 1000000000LL + (current_time.tv_nsec - start_time.tv_nsec);

        data_index++;

        // サンプリング周期に合わせて待機時間を調整
        nanosleep(&sleep_time, NULL);
    }

    // CSVファイルにデータを保存
    FILE *fp = fopen("data.csv", "w");
    if (fp != NULL)
    {
        fprintf(fp, "Time,Data\n");
        for (int i = 0; i < data_index; i++)
        {
            fprintf(fp, "%lld,%f\n", time_buffer[i], data_buffer[i]);
        }
        fclose(fp);
        printf("Data saved to data.csv\n");
    }
    else
    {
        printf("Error opening file\n");
    }

    // CSVファイルを読み取って表示
    fp = fopen("data.csv", "r");
    if (fp != NULL)
    {
        printf("\nReading data from data.csv\n");
        char line[100];
        while (fgets(line, sizeof(line), fp) != NULL)
        {
            printf("%s", line);
        }
        fclose(fp);
    }
    else
    {
        printf("Error opening file\n");
    }

    DEV_ModuleExit();
    return 0;
}
