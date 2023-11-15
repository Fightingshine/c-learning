#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<math.h>

//Find a saddle point for a two dimentional array£¬
//the saddle point is the maximum element in its row,
//and the minimum element in its column.
//The two dimentional array and its size should be input from the keyboard
//Notice that the saddle point may not exist for an array, 
//if it does not exist,
//output the information that the saddle point does not exist.
//If there exists a saddle point, please output its row number and column number.

int main()
{
	int a[10][10], n, m, i, j,max1,max2,maxcollum,k,sad;
	printf("input row size:");
	scanf("%d", &n);
	printf("input column size:");
	scanf("%d", &m);
	printf("\n");
	for (i = 0; i < n; i++)
	{
		printf("input the iterms of row%d:", i);
		for (j = 0; j < m; j++)
		{
			scanf("%d", &a[i][j]);
		}
	}
	printf("\n");
	printf("the array you input is:\n");
	for (i = 0; i < n; i++)
	{
		
		for (j = 0; j < m; j++)
		{
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
	for (i = 0; i < n; i++)
	{
		for (j = 0,max1=0; j < m; j++)
		{
			if (max1 < a[i][j])
			{
				max1 = a[i][j];
				maxcollum = j;
			}
			for (k = 0; k < n; k++)
			{
				if (max1 >a[k][maxcollum])
					sad = a[k][maxcollum];
				max2 = k;
			}
			if (max1 == sad)
			{
				printf("the saddle point is found!Its position is:array[%d][%d],and array[%d][%d] is %d", max2, maxcollum, a[max2][maxcollum]);
			}
			else
				printf("the saddle point is not found");
		}
		
	}
}