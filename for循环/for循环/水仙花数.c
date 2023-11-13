#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<math.h>


int main()
{
	int a, b, c, i;
	for (i = 100; i <= 999; i++)
	{
		a = i / pow(10, 2);
		b = (i - a * pow(10, 2)) / 10;
		c = i - a * pow(10, 2) - b * 10;
		if (i == pow(a, 3) + pow(b, 3) + pow(c, 3))
			printf("%10d", i);

	}