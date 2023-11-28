/*
 * SPI テスト ユーティリティ (spidev ドライバーを使用)
 *
 * 著作権 (c) 2007 MontaVista Software, Inc.
 * 著作権 (c) 2007 Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * このプログラムはフリーソフトウェアです。再配布および/または変更することができます
 * これは、GNU General Public License の条件に基づいて発行されます。
 * フリーソフトウェア財団。ライセンスのバージョン 2 のいずれか。
 *
 * Cross-gcc -I/path/to/cross-kernel/include によるクロスコンパイル
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void pabort(const char *s)
{
	エラー;
	アボート（）;
}

static const char *device = "/dev/spidev1.1";
静的 uint8_t モード;
静的 uint8_t ビット = 8;
静的 uint32_t 速度 = 500000;
静的 uint16_t 遅延。

静的ボイド転送(int fd)
{
	int ret;
	uint8_t tx[] = {
		0xFF、0xFF、0xFF、0xFF、0xFF、0xFF、
		0x40、0x00、0x00、0x00、0x00、0x95、
		0xFF、0xFF、0xFF、0xFF、0xFF、0xFF、
		0xFF、0xFF、0xFF、0xFF、0xFF、0xFF、
		0xFF、0xFF、0xFF、0xFF、0xFF、0xFF、
		0xDE、0xAD、0xBE、0xEF、0xBA、0xAD、
		0xF0、0x0D、
	};
	uint8_t rx[ARRAY_SIZE(tx)] = {0, };
	struct spi_ioc_transfer tr = {
		.tx_buf = (符号なしロング)tx、
		.rx_buf = (符号なし長)rx、
		.len = ARRAY_SIZE(tx)、
		.lay_usecs = 遅延、
		.speed_hz = 速度、
		.bits_per_word = ビット、
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("spi メッセージを送信できません");

	for (ret = 0; ret < ARRAY_SIZE(tx); ret++) {
		if (!(ret % 6))
			put("");
		printf("%.2X ", rx[ret]);
	}
	put("");
}

static void print_usage(const char *prog)
{
	printf("使用法: %s [-DsbdlHOLC3]\n", prog);
	put(" -D --device 使用するデバイス (デフォルト /dev/spidev1.1)\n"
	     " -s --speed 最大速度 (Hz)\n"
	     " -d --lay 遅延 (usec)\n"
	     " -b --bpw ワードあたりのビット数 \n"
	     " -l --loop ループバック\n"
	     " -H --cpha クロック位相\n"
	     " -O --cpol クロック極性\n"
	     " -L --lsb 最下位ビットが最初\n"
	     " -C --cs-high チップ選択アクティブ High\n"
	     " -3 --3wire SI/SO 信号が共有されました\n");
	終了(1);
}

static void parse_opts(int argc, char *argv[])
{
	一方 (1) {
		static const struct オプション lopts[] = {
			{ "デバイス", 1, 0, 'D' },
			{ "速度", 1, 0, 's' },
			{ "遅延", 1, 0, 'd' },
			{ "bpw", 1, 0, 'b' },
			{ "ループ", 0, 0, 'l' },
			{ "cpha", 0, 0, 'H' },
			{ "cpol", 0, 0, 'O' },
			{ "lsb", 0, 0, 'L' },
			{ "cs-high", 0, 0, 'C' },
			{ "3wire", 0, 0, '3' },
			{ "no-cs", 0, 0, 'N' },
			{ "準備完了", 0, 0, 'R' },
			{ NULL, 0, 0, 0 },
		};
		int c;

		c = getopt_long(argc, argv, "D:s:d:b:lHOLC3NR", lopts, NULL);

		if (c == -1)
			壊す;

		スイッチ (c) {
		ケース「D」:
			デバイス = オプターグ;
			壊す;
		ケース:
			速度 = atoi(optarg);
			壊す;
		ケース「d」:
			遅延 = atoi(optarg);
			壊す;
		ケース「b」:
			ビット = atoi(optarg);
			壊す;
		ケース「l」:
			モード |= SPI_LOOP;
			壊す;
		ケース「H」:
			モード |= SPI_CPHA;
			壊す;
		ケース「O」:
			モード |= SPI_CPOL;
			壊す;
		ケース「L」:
			モード |= SPI_LSB_FIRST;
			壊す;
		ケース「C」:
			モード |= SPI_CS_HIGH;
			壊す;
		ケース「3」:
			モード |= SPI_3WIRE;
			壊す;
		ケース「N」:
			モード |= SPI_NO_CS;
			壊す;
		ケース「R」:
			モード |= SPI_READY;
			壊す;
		デフォルト：
			print_usage(argv[0]);
			壊す;
		}
	}
}

int main(int argc, char *argv[])
{
	int ret = 0;
	int fd;

	parse_opts(argc, argv);

	fd = オープン(デバイス, O_RDWR);
	if (fd < 0)
		pabort("デバイスを開けません");

	/*
	 * スパイモード
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("spi モードを設定できません");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("spi モードを取得できません");

	/*
	 * ワードあたりのビット数
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("ワードあたりのビット数を設定できません");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("ワードあたりのビット数を取得できません");

	/*
	 * 最大速度 Hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("最大速度 hz を設定できません");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("最大速度 hz を取得できません");

	printf("spi モード: %d\n", モード);
	printf("ワードあたりのビット数: %d\n", ビット);
	printf("最大速度: %d Hz (%d KHz)\n", 速度, 速度/1000);

	転送(fd);

	閉じる(fd);

	retを返します。
}