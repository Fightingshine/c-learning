#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<math.h>

int min()
{
	int sum, i, j;
	for (i = 1; i <= 1000; i++)
	{
		for (sum = 0, j = 1; j <= i - 1; j++)
		{
			if (i % j == 0)
				sum = sum + j;
		}
		if (sum == i)
		{
			printf("perfect number is %d and its factors are ", sum);
			for (j = 1; j <= i - 1; j++)
			{
				if (i % j == 0)
					printf("%d ", j);
			};
			printf("\n");
		}
	}
}