#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<math.h>

int mai()
{
	int arr[100][100], m, n,i,j;
	printf("input the size of array m*n ");
	scanf("%d*%d", &m, &n);
	printf("input the iterms of the array\n");
	for (i = 0; i < m; i++)
	{
		for (j = 0; j< n; j++)
		{
			scanf("%d", &arr[i][j]);
			
		}
	}
	printf("the input array is \n");
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n;j++)
		{
			printf("%d ", arr[i][j]);
		}
		printf("\n");
	}
}