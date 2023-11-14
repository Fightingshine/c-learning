#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<math.h>

int man()
{
	int num, a[1000] = { 0 }, n, count, i, sum = 0, x;
	printf("input a number:");
	scanf("%d", &num);
	n = num;
	for (count = 0; n != 0; count++)
	{
		n = n / 10;
	}
	for (i = 0; i <= count - 1; i++)
	{
		a[i] = num % 10;
		num = num / 10;
	}
	printf("%d\n", count);
	x = count;
	for (i = 0; i <= (count - 1) / 2; i++, x--)
	{
		sum = sum + a[i] * a[i] + a[x - 1] * a[x - 1];
		printf("%d*%d+%d*%d=%d,sum=%d\n", a[i], a[i], a[x - 1], a[x - 1], a[i] * a[i] + a[x - 1] * a[x - 1], sum);
	}
	printf("summary = %d", sum);
}