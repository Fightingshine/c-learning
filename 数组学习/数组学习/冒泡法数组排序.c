#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<math.h>

int min()
{
	int n, arr[1000] = { 0 }, i, j, k, x, l;
	printf("请输入有多少个数需要排序：");
	scanf("%d", &n);
	printf("请输入需要排序的数字（中间用空格分隔)：");
	for (i = 0; i <= n - 1; i++)
	{
		scanf("%d", &arr[i]);

	}
	for (i = 0; i <= n - 2; i++)
	{
		k = i;												//k代i为与所有项比较，可让i不变的情况下找到最小项序号
		for (j = i; j <= n - 1; j++)
		{
			if (arr[j] < arr[k])
				k = j;										//找出数组最小项
		}
		if (k != i)
		{
			x = arr[i], arr[i] = arr[k], arr[k] = x;
		}
		printf("第%d次排序的结果是：", i + 1);
		for (l = 0; l <= n - 1; l++)
		{
			printf("%d ", arr[l]);
			if (l == n - 1)
				printf("\n");
		}
	}
	printf("\n最终的排序结果是:");
	for (l = 0; l <= n - 1; l++)
	{
		printf("%d ", arr[l]);
		if (l == n - 1)
			printf("\n");
	}
}