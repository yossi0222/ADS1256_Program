#include<stdio.h>
#include<time.h>

int main() {

	struct timespec start,end;
	timespec_get(&start, TIME_UTC);

	int a,b,c;
	a=1;
	b=2;
	c=a;
	a=b;
	b=c;

	timespec_get(&end, TIME_UTC);
	long diff = end.tv_nsec - start.tv_nsec;
	
	printf("処理時間は%loナノ秒です",diff);

	return 0;
}